#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "LedManager.h"
#include "DecorativeEffect.h"
#include "PaletteManager.h"

class AuroraEffect :
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

  void setPaletteManager(
    PaletteManager* paletteManager
  );

  void setSpeed(
    uint8_t speedPercent
  );

  uint8_t getSpeed() const;



  void nextPattern();

  uint8_t getPattern() const;

  const char* getPatternName() const;
private:
  void render();

  void updateIntervalFromSpeed();

  uint8_t calculateBrightness(
    uint8_t phaseOffset
  ) const;

  LedManager* leds = nullptr;

  PaletteManager* palette = nullptr;

  uint8_t animationPhase = 0;

  uint8_t palettePosition = 0;

  uint8_t speed = 50;

  

  static constexpr uint8_t PATTERN_COUNT = 2;

  uint8_t patternIndex = 0;
  uint16_t updateInterval = 30;

  unsigned long lastUpdateTime = 0;
};