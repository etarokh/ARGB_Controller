#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "AnimatedHueEffect.h"

class RainbowEffect :
  public AnimatedHueEffect
{
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
};
