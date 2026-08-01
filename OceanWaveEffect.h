#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "LedManager.h"
#include "DecorativeEffect.h"
#include "PaletteManager.h"

class OceanWaveEffect :
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

private:
  void render();

  void updateIntervalFromSpeed();

  uint8_t calculateWaveBrightness(
    uint8_t groupPosition
  ) const;

  LedManager* leds = nullptr;

  PaletteManager* palette = nullptr;

  uint8_t wavePosition = 0;

  int8_t waveDirection = 1;

  uint8_t palettePosition = 0;

  uint8_t speed = 50;

  uint16_t updateInterval = 30;

  unsigned long lastUpdateTime = 0;
};