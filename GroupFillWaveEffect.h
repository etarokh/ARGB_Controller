#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "DecorativeEffect.h"
#include "LedManager.h"
#include "PaletteManager.h"

class GroupFillWaveEffect : public DecorativeEffect
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

  void setPaletteManager(
    PaletteManager* manager
  );

  void nextPattern();

  uint8_t getPattern() const;

  const char* getPatternName() const;

private:
  enum class Phase
  {
    Filling,
    Emptying
  };

  static constexpr uint8_t GROUP_COUNT = 4;

  LedManager* leds = nullptr;

  PaletteManager* paletteManager = nullptr;

  Phase phase = Phase::Filling;

  // 0 = HDD
  // 1 = CPU
  // 2 = Intake
  // 3 = Exhaust
  uint8_t startGroup = 0;

  // Position inside the rotated group order.
  uint8_t currentGroup = 0;

  uint8_t currentLed = 0;

  uint8_t colorPosition = 0;

  uint8_t speed = 50;

  unsigned long lastUpdateTime = 0;

  unsigned long updateIntervalMs = 100;

  void stepFill();

  void stepEmpty();

  void nextCycle();

  void clearAllPixels();

  void renderPixel(
    uint8_t group,
    uint8_t led,
    bool on
  );

  uint8_t getActualGroup() const;

  uint8_t getGroupLedCount(
    uint8_t group
  ) const;

  CRGB* getGroupLeds(
    uint8_t group
  ) const;

  CRGB currentColor() const;

  void updateInterval();
};
