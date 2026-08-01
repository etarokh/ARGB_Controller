#pragma once

#include <FastLED.h>
#include "Config.h"

class LedManager
{
public:
  void begin();

  void setAll(const CRGB& color);

  void setHdd(const CRGB& color);
  void setIntake(const CRGB& color);
  void setExhaust(const CRGB& color);
  void setCpu(const CRGB& color);

  CRGB* getHddLeds();
  CRGB* getIntakeLeds();
  CRGB* getExhaustLeds();
  CRGB* getCpuLeds();

  void show();
  void clear();

  void setBrightness(
    uint8_t brightness
  );

  uint8_t getBrightness() const;

  void setEffectBrightness(
    uint8_t brightness
  );

  uint8_t getEffectBrightness() const;

private:
  CRGB hddLeds[LED_COUNT_HDD];
  CRGB intakeLeds[LED_COUNT_INTAKE];
  CRGB exhaustLeds[LED_COUNT_EXHAUST];
  CRGB cpuLeds[LED_COUNT_CPU];

  uint8_t globalBrightness =
    DEFAULT_BRIGHTNESS;

  uint8_t effectBrightness = 255;

  void applyBrightness();
};
