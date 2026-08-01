#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "AnimatedHueEffect.h"

class PlasmaEffect : public AnimatedHueEffect
{
public:
  void nextPattern();
  uint8_t getPattern() const;
  const char* getPatternName() const;

protected:
  void renderFrame() override;

  SpeedProfile getSpeedProfile() const override
  {
    return SpeedProfile::Plasma;
  }

  uint8_t getHueStep() const override
  {
    return 2;
  }

private:
  static constexpr uint8_t PATTERN_COUNT = 4;
  uint8_t patternIndex = 0;
};
