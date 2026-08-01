#pragma once

#include <Arduino.h>

class LedManager;

class BrightnessManager
{
public:
  void begin(
    LedManager* ledManager
  );

  void setBrightness(
    uint8_t newBrightness
  );

  uint8_t getBrightness() const;

  void increase();
  void decrease();

  void showPreview();

  void updateAdjustment(
    bool adjusting
  );

private:
  static constexpr uint8_t MIN_BRIGHTNESS = 10;
  static constexpr uint8_t MAX_BRIGHTNESS = 255;
  static constexpr uint8_t BRIGHTNESS_STEP = 5;

  static constexpr unsigned long
    ADJUST_INTERVAL_MS = 45UL;

  LedManager* leds = nullptr;

  uint8_t brightness = 100;

  bool increasing = false;
  bool wasAdjusting = false;

  unsigned long lastAdjustmentAt = 0;

  void apply();
  void flashBoundary();
};
