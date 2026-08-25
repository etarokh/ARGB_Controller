#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "AnimatedHueEffect.h"

class RainbowEffect :
  public AnimatedHueEffect
{
public:
  void nextPattern();

  uint8_t getPattern() const;

  const char* getPatternName() const;

protected:
  void renderFrame() override;

  SpeedProfile
  getSpeedProfile() const override
  {
    return SpeedProfile::Rainbow;
  }

  uint8_t getHueStep() const override
  {
    return 1;
  }

private:
  static constexpr uint8_t
    PATTERN_COUNT = 2;

  uint8_t patternIndex = 0;
};
