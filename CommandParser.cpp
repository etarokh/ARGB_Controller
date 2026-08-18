#include "CommandParser.h"
#include "SettingsStorage.h"

#include <stdlib.h>
#include <string.h>

void CommandParser::begin(
  EffectManager* effectManager,
  ModeManager* modeManager,
  MonitorManager* monitorManager,
  DecorativeEffectManager* decorativeManager,
  BrightnessManager* brightnessManager
)
{
  effects = effectManager;
  modes = modeManager;
  monitor = monitorManager;
  decorative = decorativeManager;
  brightness = brightnessManager;

  inputLength = 0;
  bufferOverflow = false;
  inputBuffer[0] = '\0';

  Serial.println(
    "SERIAL_PARSER_READY"
  );

  Serial.println(
    "READY"
  );
}

void CommandParser::update()
{
  while (Serial.available() > 0)
  {
    const char incoming =
      static_cast<char>(Serial.read());

    if (incoming == '\r')
    {
      continue;
    }

    if (incoming == '\n')
    {
      if (bufferOverflow)
      {
        printError(
          "COMMAND_TOO_LONG"
        );
      }
      else if (inputLength > 0)
      {
        inputBuffer[inputLength] = '\0';
        processLine(inputBuffer);
      }

      inputLength = 0;
      bufferOverflow = false;
      inputBuffer[0] = '\0';

      continue;
    }

    if (bufferOverflow)
    {
      continue;
    }

    if (
      inputLength <
      BUFFER_SIZE - 1
    )
    {
      inputBuffer[inputLength] =
        incoming;

      inputLength++;
    }
    else
    {
      bufferOverflow = true;
    }
  }
}

void CommandParser::processLine(
  char* line
)
{
  if (effects == nullptr)
  {
    printError(
      "PARSER_NOT_INITIALIZED"
    );
    return;
  }

  if (processCommand(line))
  {
    return;
  }

  MonitorState cpuHardwareState = MonitorState::Normal;
  MonitorState cpuSoftwareState = MonitorState::Normal;
  MonitorState cpuSystemState = MonitorState::Normal;

  MonitorState serviceHardwareState = MonitorState::Normal;
  MonitorState serviceSoftwareState = MonitorState::Normal;
  MonitorState serviceSystemState = MonitorState::Normal;

  MonitorState networkState;

  MonitorState diskHardwareStates[
    EffectManager::DISK_COUNT
  ];

  MonitorState diskSoftwareStates[
    EffectManager::DISK_COUNT
  ];

  MonitorState diskSystemStates[
    EffectManager::DISK_COUNT
  ] = {
    MonitorState::Normal,
    MonitorState::Normal,
    MonitorState::Normal,
    MonitorState::Normal
  };

  bool cpuHardwareFound = false;
  bool cpuSoftwareFound = false;
  bool cpuSystemFound = false;

  bool serviceHardwareFound = false;
  bool serviceSoftwareFound = false;
  bool serviceSystemFound = false;

  bool networkFound = false;
  bool timeFound = false;

  uint64_t unixTime = 0;

  uint8_t cpuUsagePercent = 0;
  uint8_t storageUsedPercent = 0;

  float storageFreeTb = 0.0f;
  float storageTotalTb = 0.0f;

  bool cpuUsageFound = false;
  bool storageUsedFound = false;
  bool storageFreeFound = false;
  bool storageTotalFound = false;

  bool diskHardwareFound[
    EffectManager::DISK_COUNT
  ] = {
    false,
    false,
    false,
    false
  };

  bool diskSoftwareFound[
    EffectManager::DISK_COUNT
  ] = {
    false,
    false,
    false,
    false
  };

  bool diskSystemFound[
    EffectManager::DISK_COUNT
  ] = {
    false,
    false,
    false,
    false
  };

  char* savePointer = nullptr;

  char* token = strtok_r(
    line,
    ";",
    &savePointer
  );

  while (token != nullptr)
  {
    char* separator =
      strchr(token, '=');

    if (separator == nullptr)
    {
      printError(
        "INVALID_TOKEN"
      );
      return;
    }

    *separator = '\0';

    const char* key = token;
    const char* value =
      separator + 1;

    if (strcmp(key, "TIME") == 0)
    {
      if (timeFound)
      {
        printError(
          "DUPLICATE_TIME"
        );
        return;
      }

      if (
        !parseUnixTime(
          value,
          unixTime
        )
      )
      {
        printError(
          "INVALID_TIME"
        );
        return;
      }

      timeFound = true;
    }
    else if (strcmp(key, "CPUPCT") == 0)
    {
      if (
        cpuUsageFound ||
        !parsePercent(
          value,
          cpuUsagePercent
        )
      )
      {
        printError(
          cpuUsageFound
            ? "DUPLICATE_CPUPCT"
            : "INVALID_CPUPCT"
        );

        return;
      }

      cpuUsageFound = true;
    }
    else if (strcmp(key, "DBUSED") == 0)
    {
      if (
        storageUsedFound ||
        !parsePercent(
          value,
          storageUsedPercent
        )
      )
      {
        printError(
          storageUsedFound
            ? "DUPLICATE_DBUSED"
            : "INVALID_DBUSED"
        );

        return;
      }

      storageUsedFound = true;
    }
    else if (strcmp(key, "DBFREE") == 0)
    {
      if (
        storageFreeFound ||
        !parsePositiveFloat(
          value,
          storageFreeTb
        )
      )
      {
        printError(
          storageFreeFound
            ? "DUPLICATE_DBFREE"
            : "INVALID_DBFREE"
        );

        return;
      }

      storageFreeFound = true;
    }
    else if (strcmp(key, "DBTOTAL") == 0)
    {
      if (
        storageTotalFound ||
        !parsePositiveFloat(
          value,
          storageTotalTb
        ) ||
        storageTotalTb <= 0.0f
      )
      {
        printError(
          storageTotalFound
            ? "DUPLICATE_DBTOTAL"
            : "INVALID_DBTOTAL"
        );

        return;
      }

      storageTotalFound = true;
    }
    else
    {
      MonitorState parsedState;

      if (
        !parseState(
          value,
          parsedState
        )
      )
      {
        printError(
          "INVALID_STATE"
        );
        return;
      }

      if (strcmp(key, "CPUHW") == 0)
      {
        cpuHardwareState = parsedState;
        cpuHardwareFound = true;
      }
      else if (strcmp(key, "CPUSW") == 0)
      {
        cpuSoftwareState = parsedState;
        cpuSoftwareFound = true;
      }
      else if (strcmp(key, "CPUSYS") == 0)
      {
        cpuSystemState = parsedState;
        cpuSystemFound = true;
      }
      else if (strcmp(key, "NET") == 0)
      {
        if (networkFound)
        {
          printError(
            "DUPLICATE_NET"
          );
          return;
        }

        networkState = parsedState;
        networkFound = true;
      }
      else if (strcmp(key, "SERVICEHW") == 0)
      {
        serviceHardwareState = parsedState;
        serviceHardwareFound = true;
      }
      else if (strcmp(key, "SERVICESW") == 0)
      {
        serviceSoftwareState = parsedState;
        serviceSoftwareFound = true;
      }
      else if (strcmp(key, "SERVICESYS") == 0)
      {
        serviceSystemState = parsedState;
        serviceSystemFound = true;
      }
      else if (
        key[0] == 'B' &&
        key[1] >= '1' &&
        key[1] <= '4' &&
        key[2] == 'H' &&
        key[3] == 'W'
      )
      {
        const uint8_t diskIndex =
          static_cast<uint8_t>(
            key[1] - '1'
          );

        diskHardwareStates[diskIndex] =
          parsedState;

        diskHardwareFound[diskIndex] =
          true;
      }
      else if (
        key[0] == 'B' &&
        key[1] >= '1' &&
        key[1] <= '4' &&
        key[2] == 'S' &&
        key[3] == 'W'
      )
      {
        const uint8_t diskIndex =
          static_cast<uint8_t>(
            key[1] - '1'
          );

        diskSoftwareStates[diskIndex] =
          parsedState;

        diskSoftwareFound[diskIndex] =
          true;
      }
      else if (
        key[0] == 'B' &&
        key[1] >= '1' &&
        key[1] <= '4' &&
        key[2] == 'S' &&
        key[3] == 'Y' &&
        key[4] == 'S'
      )
      {
        const uint8_t diskIndex =
          static_cast<uint8_t>(
            key[1] - '1'
          );

        diskSystemStates[diskIndex] =
          parsedState;

        diskSystemFound[diskIndex] =
          true;
      }
      else
      {
        printError(
          "UNKNOWN_KEY"
        );
        return;
      }
    }

    token = strtok_r(
      nullptr,
      ";",
      &savePointer
    );
  }

  if (
    !cpuHardwareFound ||
    !cpuSoftwareFound ||
    !networkFound
  )
  {
    printError(
      "INCOMPLETE_SNAPSHOT"
    );
    return;
  }

  for (
    uint8_t i = 0;
    i < EffectManager::DISK_COUNT;
    i++
  )
  {
    if (
      !diskHardwareFound[i] ||
      !diskSoftwareFound[i]
    )
    {
      printError(
        "INCOMPLETE_SNAPSHOT"
      );
      return;
    }
  }

  if (
    timeFound &&
    modes != nullptr
  )
  {
    modes->setUnixTime(
      unixTime
    );
  }

  if (monitor != nullptr)
  {
    if (cpuUsageFound)
    {
      monitor->setCpuUsagePercent(
        cpuUsagePercent
      );
    }

    if (storageUsedFound)
    {
      monitor->setStorageUsedPercent(
        storageUsedPercent
      );
    }

    if (
      storageFreeFound ||
      storageTotalFound
    )
    {
      const float freeTb =
        storageFreeFound
          ? storageFreeTb
          : monitor->getStorageFreeTb();

      const float totalTb =
        storageTotalFound
          ? storageTotalTb
          : monitor->getStorageTotalTb();

      monitor->setStorageCapacity(
        freeTb,
        totalTb
      );
    }
  }

  effects->setCpuHardwareState(
    cpuHardwareState
  );

  effects->setCpuSoftwareState(
    cpuSoftwareState
  );

  effects->setCpuSystemState(
    cpuSystemState
  );

  effects->setNetworkState(
    networkState
  );

  effects->setServiceHardwareState(
    serviceHardwareState
  );

  effects->setServiceSoftwareState(
    serviceSoftwareState
  );

  effects->setServiceSystemState(
    serviceSystemState
  );

  for (
    uint8_t i = 0;
    i < EffectManager::DISK_COUNT;
    i++
  )
  {
    effects->setDiskHardwareState(
      i,
      diskHardwareStates[i]
    );

    effects->setDiskSoftwareState(
      i,
      diskSoftwareStates[i]
    );

    effects->setDiskSystemState(
      i,
      diskSystemStates[i]
    );
  }

  Serial.println(
    "ACK"
  );
}

bool CommandParser::processCommand(
  char* line
)
{
  if (strcmp(line, "PING") == 0)
  {
    printOk();
    return true;
  }

  if (strcmp(line, "GET_STATUS") == 0)
  {
    Serial.print("OK STATUS ");

    Serial.print("MODE=");
    Serial.print(
      settingsStorage().getMode()
    );

    Serial.print(";EFFECT=");
    Serial.print(
      settingsStorage().
        getDecorativeEffect()
    );

    Serial.print(";BRIGHTNESS=");
    Serial.print(
      settingsStorage().getBrightness()
    );

    Serial.print(";SPEED=");
    Serial.print(
      settingsStorage().
        getDecorativeSpeed()
    );

    Serial.print(";COLOR=");
    Serial.print(
      settingsStorage().
        getStaticColorIndex()
    );

    Serial.print(";BASECOLOR=");
    Serial.print(
      monitor == nullptr
        ? 0
        : monitor->
            getBaseGaugeColorIndex()
    );

    Serial.print(";CPUCOLOR=");
    Serial.print(
      monitor == nullptr
        ? 0
        : monitor->
            getCpuGaugeColorIndex()
    );

    Serial.print(";STORAGECOLOR=");
    Serial.print(
      monitor == nullptr
        ? 0
        : monitor->
            getStorageGaugeColorIndex()
    );

    Serial.print(";PATTERN=");
    Serial.print(
      decorative == nullptr
        ? 0
        : decorative->getPattern()
    );

    Serial.print(";PATTERNCOUNT=");
    Serial.print(
      decorative == nullptr
        ? 0
        : decorative->getPatternCount()
    );

    Serial.print(";HASPATTERN=");
    Serial.println(
      decorative != nullptr &&
      decorative->hasPattern()
        ? 1
        : 0
    );

    return true;
  }

  if (
    strncmp(
      line,
      "SET_MODE ",
      9
    ) == 0
  )
  {
    if (modes == nullptr)
    {
      printCommandError(
        "MODE_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 9;

    SystemMode requestedMode;

    if (
      strcmp(
        value,
        "DECORATIVE"
      ) == 0
    )
    {
      requestedMode =
        SystemMode::Decorative;
    }
    else if (
      strcmp(
        value,
        "MONITOR"
      ) == 0
    )
    {
      requestedMode =
        SystemMode::Monitor;
    }
    else if (
      strcmp(
        value,
        "OFF"
      ) == 0
    )
    {
      requestedMode =
        SystemMode::Off;
    }
    else
    {
      printCommandError(
        "INVALID_MODE"
      );

      return true;
    }

    modes->setMode(
      requestedMode
    );

    printOk();

    return true;
  }

  if (
    strncmp(
      line,
      "SET_EFFECT ",
      11
    ) == 0
  )
  {
    if (decorative == nullptr)
    {
      printCommandError(
        "DECORATIVE_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 11;
    char* endPointer = nullptr;

    const long effectIndex =
      strtol(
        value,
        &endPointer,
        10
      );

    if (
      endPointer == value ||
      *endPointer != '\0' ||
      effectIndex < 0 ||
      effectIndex > 16
    )
    {
      printCommandError(
        "INVALID_EFFECT"
      );

      return true;
    }

    decorative->setEffect(
      static_cast<DecorativeEffectType>(
        effectIndex
      )
    );

    printOk();

    return true;
  }

  if (
    strncmp(
      line,
      "SET_PATTERN ",
      12
    ) == 0
  )
  {
    if (decorative == nullptr)
    {
      printCommandError(
        "DECORATIVE_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 12;
    char* endPointer = nullptr;

    const long patternIndex =
      strtol(
        value,
        &endPointer,
        10
      );

    if (
      endPointer == value ||
      *endPointer != '\0' ||
      patternIndex < 0 ||
      !decorative->hasPattern() ||
      patternIndex >=
        decorative->getPatternCount()
    )
    {
      printCommandError(
        "INVALID_PATTERN"
      );

      return true;
    }

    decorative->setPattern(
      static_cast<uint8_t>(
        patternIndex
      )
    );

    printOk();

    return true;
  }

  if (
    strncmp(
      line,
      "SET_BRIGHTNESS ",
      15
    ) == 0
  )
  {
    if (brightness == nullptr)
    {
      printCommandError(
        "BRIGHTNESS_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 15;
    char* endPointer = nullptr;

    const long brightnessValue =
      strtol(
        value,
        &endPointer,
        10
      );

    if (
      endPointer == value ||
      *endPointer != '\0' ||
      brightnessValue < 10 ||
      brightnessValue > 255
    )
    {
      printCommandError(
        "INVALID_BRIGHTNESS"
      );

      return true;
    }

    brightness->setBrightness(
      static_cast<uint8_t>(
        brightnessValue
      )
    );

    settingsStorage().setBrightness(
      static_cast<uint8_t>(
        brightnessValue
      )
    );

    printOk();

    return true;
  }

  if (
    strncmp(
      line,
      "SET_SPEED ",
      10
    ) == 0
  )
  {
    if (decorative == nullptr)
    {
      printCommandError(
        "DECORATIVE_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 10;
    char* endPointer = nullptr;

    const long speedValue =
      strtol(
        value,
        &endPointer,
        10
      );

    if (
      endPointer == value ||
      *endPointer != '\0' ||
      speedValue < 1 ||
      speedValue > 100
    )
    {
      printCommandError(
        "INVALID_SPEED"
      );

      return true;
    }

    decorative->setSpeed(
      static_cast<uint8_t>(
        speedValue
      )
    );

    settingsStorage().
      setDecorativeSpeed(
        static_cast<uint8_t>(
          speedValue
        )
      );

    printOk();

    return true;
  }

  if (
    strncmp(
      line,
      "SET_COLOR ",
      10
    ) == 0
  )
  {
    if (decorative == nullptr)
    {
      printCommandError(
        "DECORATIVE_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 10;
    char* endPointer = nullptr;

    const long colorIndex =
      strtol(
        value,
        &endPointer,
        10
      );

    if (
      endPointer == value ||
      *endPointer != '\0' ||
      colorIndex < 0 ||
      colorIndex > 29
    )
    {
      printCommandError(
        "INVALID_COLOR"
      );

      return true;
    }

    decorative->setStaticColorIndex(
      static_cast<uint8_t>(
        colorIndex
      )
    );

    printOk();

    return true;
  }

  if (
    strncmp(
      line,
      "SET_CPU_GAUGE_COLOR ",
      20
    ) == 0
  )
  {
    if (monitor == nullptr)
    {
      printCommandError(
        "MONITOR_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 20;
    char* endPointer = nullptr;

    const long colorIndex =
      strtol(
        value,
        &endPointer,
        10
      );

    if (
      endPointer == value ||
      *endPointer != '\0' ||
      colorIndex < 0 ||
      colorIndex >=
        MonitorManager::THEME_COUNT
    )
    {
      printCommandError(
        "INVALID_CPU_GAUGE_COLOR"
      );

      return true;
    }

    monitor->
      setCpuGaugeColorIndex(
        static_cast<uint8_t>(
          colorIndex
        )
      );

    printOk();

    return true;
  }

  if (
    strncmp(
      line,
      "SET_STORAGE_GAUGE_COLOR ",
      24
    ) == 0
  )
  {
    if (monitor == nullptr)
    {
      printCommandError(
        "MONITOR_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 24;
    char* endPointer = nullptr;

    const long colorIndex =
      strtol(
        value,
        &endPointer,
        10
      );

    if (
      endPointer == value ||
      *endPointer != '\0' ||
      colorIndex < 0 ||
      colorIndex >=
        MonitorManager::THEME_COUNT
    )
    {
      printCommandError(
        "INVALID_STORAGE_GAUGE_COLOR"
      );

      return true;
    }

    monitor->
      setStorageGaugeColorIndex(
        static_cast<uint8_t>(
          colorIndex
        )
      );

    printOk();

    return true;
  }

  if (
    strncmp(
      line,
      "SET_BASE_GAUGE_COLOR ",
      21
    ) == 0
  )
  {
    if (monitor == nullptr)
    {
      printCommandError(
        "MONITOR_MANAGER_UNAVAILABLE"
      );

      return true;
    }

    const char* value = line + 21;
    char* endPointer = nullptr;

    const long colorIndex =
      strtol(
        value,
        &endPointer,
        10
      );

    if (
      endPointer == value ||
      *endPointer != '\0' ||
      colorIndex < 0 ||
      colorIndex >=
        MonitorManager::THEME_COUNT
    )
    {
      printCommandError(
        "INVALID_BASE_GAUGE_COLOR"
      );

      return true;
    }

    monitor->
      setBaseGaugeColorIndex(
        static_cast<uint8_t>(
          colorIndex
        )
      );

    printOk();

    return true;
  }

  if (strchr(line, '=') == nullptr)
  {
    printCommandError(
      "UNKNOWN_COMMAND"
    );

    return true;
  }

  return false;
}

void CommandParser::printOk() const
{
  Serial.println(
    "OK"
  );
}

void CommandParser::printCommandError(
  const char* message
) const
{
  Serial.print(
    "ERROR:"
  );

  Serial.println(
    message
  );
}

bool CommandParser::parseState(
  const char* value,
  MonitorState& state
) const
{
  if (
    value == nullptr ||
    value[0] == '\0' ||
    value[1] != '\0'
  )
  {
    return false;
  }

  switch (value[0])
  {
    case 'N':
      state =
        MonitorState::Normal;
      return true;

    case 'W':
      state =
        MonitorState::Warning;
      return true;

    case 'C':
      state =
        MonitorState::Critical;
      return true;

    case 'O':
      state =
        MonitorState::Offline;
      return true;

    default:
      return false;
  }
}

bool CommandParser::parseUnixTime(
  const char* value,
  uint64_t& unixTime
) const
{
  if (
    value == nullptr ||
    value[0] == '\0'
  )
  {
    return false;
  }

  char* endPointer = nullptr;

  unsigned long long parsed =
    strtoull(
      value,
      &endPointer,
      10
    );

  if (
    endPointer == value ||
    *endPointer != '\0' ||
    parsed < 1000000000ULL
  )
  {
    return false;
  }

  unixTime =
    static_cast<uint64_t>(
      parsed
    );

  return true;
}

bool CommandParser::parsePercent(
  const char* value,
  uint8_t& percent
) const
{
  if (
    value == nullptr ||
    value[0] == '\0'
  )
  {
    return false;
  }

  char* endPointer = nullptr;

  const long parsed =
    strtol(
      value,
      &endPointer,
      10
    );

  if (
    endPointer == value ||
    *endPointer != '\0' ||
    parsed < 0 ||
    parsed > 100
  )
  {
    return false;
  }

  percent =
    static_cast<uint8_t>(
      parsed
    );

  return true;
}

bool CommandParser::parsePositiveFloat(
  const char* value,
  float& parsedValue
) const
{
  if (
    value == nullptr ||
    value[0] == '\0'
  )
  {
    return false;
  }

  char* endPointer = nullptr;

  const float parsed =
    strtof(
      value,
      &endPointer
    );

  if (
    endPointer == value ||
    *endPointer != '\0' ||
    parsed < 0.0f ||
    parsed > 1000000.0f
  )
  {
    return false;
  }

  parsedValue = parsed;

  return true;
}

void CommandParser::printError(
  const char* message
) const
{
  Serial.print(
    "ERR:"
  );

  Serial.println(
    message
  );
}
