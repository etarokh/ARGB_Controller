#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "DecorativeEffect.h"
#include "LedManager.h"

class BreathingEffect : public DecorativeEffect
{
public:
  void begin(
    LedManager* ledManager
  ) override;

  void update() override;

  void reset() override;

  void setColor(
    const CRGB& color
  ) override;

  void setSpeed(
    uint8_t speedPercent
  );

  uint8_t getSpeed() const;

private:
  LedManager* leds = nullptr;

  CRGB effectColor = CRGB::Blue;

  uint8_t currentBrightness = 15;
  int8_t brightnessDirection = 1;

  uint8_t speed = 50;

  unsigned long lastUpdateTime = 0;
  unsigned long updateIntervalMs = 20;

  static const uint8_t MIN_BRIGHTNESS = 15;
  static const uint8_t BRIGHTNESS_STEP = 2;

  void updateInterval();
};
