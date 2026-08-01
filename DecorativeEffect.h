#pragma once

#include <Arduino.h>
#include <FastLED.h>

class LedManager;

class DecorativeEffect {
public:
  virtual void begin(LedManager* ledManager) = 0;

  virtual void update() = 0;

  virtual void reset() = 0;

  virtual void setColor(const CRGB& color) = 0;
};