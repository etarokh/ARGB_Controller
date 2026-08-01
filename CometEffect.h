#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "DecorativeEffect.h"
#include "LedManager.h"

class CometEffect : public DecorativeEffect
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
  static constexpr uint8_t TAIL_LENGTH = 18;

  LedManager* leds = nullptr;

  int16_t headPosition = 0;

  int8_t movementDirection = 1;

  uint8_t hue = 0;

  uint8_t speed = 50;

  unsigned long lastUpdateTime = 0;

  unsigned long updateIntervalMs = 100;

  void advanceWave();

  void render();

  void clearAllPixels();

  void setGlobalPixel(
    uint16_t globalIndex,
    const CRGB& color
  );

  void updateInterval();
};
