#pragma once

#include <Arduino.h>
#include <Preferences.h>

#include "LedManager.h"
#include "EffectManager.h"
#include "DecorativeEffectManager.h"
#include "MonitorManager.h"
#include "BuzzerManager.h"
#include "BrightnessManager.h"

enum class SystemMode {
  Decorative,
  Settings,
  Monitor,
  Off
};

class ModeManager {
public:
  void begin(
    LedManager* ledManager,
    EffectManager* monitoringManager,
    DecorativeEffectManager* decorativeManager,
    MonitorManager* monitorModeManager,
    BuzzerManager* buzzerManager,
    BrightnessManager* brightnessManager
  );

  void update();

  void setMode(SystemMode mode);
  SystemMode getMode() const;
  void nextMode();


  bool isAlertOverrideActive() const;

  void setUnixTime(uint64_t unixTime);
  bool hasValidUnixTime() const;

  void snoozeAlertsFor24Hours();
  void snoozeAlertsFor30Days();
  void snoozeAlertsFor6Months();
  void clearAlertSnooze();

  bool areAlertsSnoozed() const;
  uint8_t getSnoozedAlertMask() const;

  const char* getModeName() const;

private:
  void applyMode();
  void clearStoredSnooze();

  LedManager* leds = nullptr;
  EffectManager* monitoring = nullptr;
  DecorativeEffectManager* decorative = nullptr;
  MonitorManager* monitorManager = nullptr;
  BuzzerManager* buzzer = nullptr;
  BrightnessManager* brightness = nullptr;

  Preferences preferences;
  bool preferencesReady = false;

  SystemMode currentMode =
    SystemMode::Off;


  bool alertOverrideActive = false;
  bool alertSnoozeActive = false;

  uint8_t snoozedAlertMask = 0;

  bool unixTimeValid = false;
  uint64_t currentUnixTime = 0;
  uint64_t alertSnoozeUntilUnix = 0;

  uint64_t fallbackSnoozeStartedAtMs = 0;

  uint64_t alertSnoozeDurationMs = 0;

  void snoozeCurrentAlertsFor(
    uint64_t durationMs,
    const char* label
  );
};
