#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "AnimatedHueEffect.h"

class GradientFlowEffect :
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
    return SpeedProfile::GradientFlow;
  }

  uint8_t getHueStep() const override
  {
    return 1;
  }

private:
  static constexpr uint8_t
    PATTERN_COUNT = 5;

  uint8_t patternIndex = 0;

  uint8_t calculateHueOffset(
    uint8_t groupIndex,
    uint16_t ledIndex
  ) const;

  uint8_t calculateBrightness(
    uint8_t groupIndex,
    uint16_t ledIndex
  ) const;
};
