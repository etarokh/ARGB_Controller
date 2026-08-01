#!/usr/bin/env python3

import json
import time
import urllib.parse
import urllib.request

import serial


PROMETHEUS_URL = "http://localhost:9090"
SERIAL_PORT = "/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0"
SERIAL_BAUD = 115200
NETWORK_INTERFACE = "br1"

POLL_INTERVAL_SECONDS = 5
SERIAL_RECONNECT_SECONDS = 5


def prometheus_query(query):
    params = urllib.parse.urlencode({"query": query})
    url = f"{PROMETHEUS_URL}/api/v1/query?{params}"

    with urllib.request.urlopen(url, timeout=5) as response:
        payload = json.load(response)

    if payload.get("status") != "success":
        raise RuntimeError("Prometheus query failed")

    return payload["data"]["result"]


def cpu_state():
    results = prometheus_query(
        "truenas_cpu_temperature_celsius"
    )

    if not results:
        return "O", None

    temperatures = [
        float(item["value"][1])
        for item in results
    ]

    maximum = max(temperatures)

    if maximum >= 85:
        return "C", maximum

    if maximum >= 70:
        return "W", maximum

    return "N", maximum


def network_state():
    query = (
        "truenas_network_link_status"
        f'{{interface="{NETWORK_INTERFACE}"}}'
    )

    results = prometheus_query(query)

    if not results:
        return "O", None

    link_status = float(results[0]["value"][1])

    if link_status == 1:
        return "N", link_status

    return "O", link_status


def disk_states():
    query = (
        "truenas_disk_health_score"
        '{bay=~"BAY1|BAY2|BAY3|BAY4"}'
    )

    results = prometheus_query(query)

    scores = {
        item["metric"]["bay"]: float(item["value"][1])
        for item in results
    }

    states = {}

    for bay in ("BAY1", "BAY2", "BAY3", "BAY4"):
        score = scores.get(bay)

        if score is None:
            states[bay] = "O"
        elif score < 80:
            states[bay] = "C"
        elif score < 100:
            states[bay] = "W"
        else:
            states[bay] = "N"

    return states, scores


def build_command():
    cpu, cpu_temp = cpu_state()
    network, network_value = network_state()
    disks, disk_scores = disk_states()

    unix_time = int(time.time())

    command = (
        f"TIME={unix_time};"
        f"CPU={cpu};"
        f"NET={network};"
        f"B1={disks['BAY1']};"
        f"B2={disks['BAY2']};"
        f"B3={disks['BAY3']};"
        f"B4={disks['BAY4']}"
    )

    debug = {
        "cpu_max_celsius": cpu_temp,
        "network_interface": NETWORK_INTERFACE,
        "network_link": network_value,
        "disk_scores": disk_scores,
        "command": command,
    }

    return command, debug


def open_serial():
    print(f"Opening serial port: {SERIAL_PORT}")

    port = serial.Serial(
        SERIAL_PORT,
        SERIAL_BAUD,
        timeout=3,
    )

    # Opening the port may reset some ESP32 boards.
    time.sleep(2)
    port.reset_input_buffer()

    print("Serial port ready")
    return port


def send_command(port, command):
    port.reset_input_buffer()

    port.write(
        (command + "\n").encode("ascii")
    )
    port.flush()

    deadline = time.monotonic() + 8
    received = []

    while time.monotonic() < deadline:
        raw = port.readline()

        if not raw:
            continue

        line = raw.decode(
            "ascii",
            errors="ignore",
        ).strip()

        if not line:
            continue

        received.append(line)

        if "ACK" in line:
            return "ACK"

        if "ERR:" in line:
            raise RuntimeError(
                f"ESP32 rejected command: {line}"
            )

        if len(received) > 100:
            received = received[-100:]

    raise RuntimeError(
        "ACK not received from ESP32. "
        f"Last received lines: {received[-20:]!r}"
    )


def main():
    port = None
    last_command = None

    while True:
        try:
            if port is None or not port.is_open:
                port = open_serial()
                last_command = None

            command, debug = build_command()

            if command != last_command:
                print()
                print(json.dumps(debug, indent=2))
                print(f"Sending: {command}")

                response = send_command(port, command)
                print(f"ESP32 response: {response}")

                last_command = command
            else:
                print(
                    f"No change: {command}",
                    flush=True,
                )

            time.sleep(POLL_INTERVAL_SECONDS)

        except KeyboardInterrupt:
            print("\nStopping LED sender")
            break

        except Exception as error:
            print(
                f"ERROR: {type(error).__name__}: {error}",
                flush=True,
            )

            if port is not None:
                try:
                    port.close()
                except Exception:
                    pass

            port = None
            last_command = None

            time.sleep(SERIAL_RECONNECT_SECONDS)

    if port is not None and port.is_open:
        port.close()


if __name__ == "__main__":
    main()
