#pragma once

#include <Arduino.h>
#include <FastLED.h>

class GaugeRenderer
{
public:
  static constexpr uint8_t LEDS_PER_RING = 8;

  void render(
    CRGB* leds,
    uint16_t ledCount,
    uint8_t percent,
    const CRGB& gaugeColor,
    const CRGB& backgroundColor
  ) const;

private:
  CRGB blendBoundaryColor(
    const CRGB& backgroundColor,
    const CRGB& gaugeColor,
    uint8_t amount
  ) const;
};
