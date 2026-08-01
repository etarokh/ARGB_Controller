#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "DecorativeEffect.h"
#include "LedManager.h"

class AlternatingStaticEffect :
  public DecorativeEffect
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

  void nextPattern();

  uint8_t getPattern() const;

  const char* getPatternName() const;

private:
  static constexpr uint8_t
    PATTERN_COUNT = 12;

  LedManager* leds = nullptr;

  uint8_t patternIndex = 0;

  uint8_t speed = 50;

  bool needsRender = true;

  void render();

  CRGB getFirstColor() const;

  CRGB getSecondColor() const;
};
