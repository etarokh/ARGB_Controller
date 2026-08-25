#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "AnimatedHueEffect.h"

class TwinkleStarsEffect :
  public AnimatedHueEffect
{
public:
  void nextPattern();

  uint8_t getPattern() const;

  const char* getPatternName() const;

protected:
  void renderFrame() override;

  void onReset() override;

  SpeedProfile
  getSpeedProfile() const override
  {
    return SpeedProfile::TwinkleStars;
  }

  uint8_t getHueStep() const override
  {
    return 1;
  }

private:
  static constexpr uint8_t
    PATTERN_COUNT = 5;

  uint8_t patternIndex = 0;

  uint8_t getFadeAmount() const;

  uint8_t getSpawnChance() const;

  uint8_t getStarsPerFrame() const;

  uint8_t getMinimumBrightness() const;

  void fadeAllStars();

  void createRandomStars();
};
