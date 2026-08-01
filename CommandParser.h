#pragma once

#include <Arduino.h>

#include "EffectManager.h"
#include "ModeManager.h"
#include "MonitorManager.h"

class CommandParser
{
public:
  void begin(
    EffectManager* effectManager,
    ModeManager* modeManager,
    MonitorManager* monitorManager
  );

  void update();

private:
  static constexpr size_t BUFFER_SIZE = 256;

  EffectManager* effects = nullptr;
  ModeManager* modes = nullptr;
  MonitorManager* monitor = nullptr;

  char inputBuffer[BUFFER_SIZE];
  size_t inputLength = 0;
  bool bufferOverflow = false;

  void processLine(char* line);

  bool parseState(
    const char* value,
    MonitorState& state
  ) const;

  bool parseUnixTime(
    const char* value,
    uint64_t& unixTime
  ) const;

  bool parsePercent(
    const char* value,
    uint8_t& percent
  ) const;

  bool parsePositiveFloat(
    const char* value,
    float& parsedValue
  ) const;

  void printError(
    const char* message
  ) const;
};
