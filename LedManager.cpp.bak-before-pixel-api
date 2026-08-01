#include "LedManager.h"

// Physical wiring:
//
// Group 1 = GPIO14 = Intake
// Group 2 = GPIO12 = Exhaust / Network
// Group 3 = GPIO23 = HDD
// Group 4 = GPIO22 = CPU

void LedManager::begin()
{
  FastLED.addLeds<
    WS2812B,
    PIN_LED_INTAKE,
    GRB
  >(
    intakeLeds,
    LED_COUNT_INTAKE
  );

  FastLED.addLeds<
    WS2812B,
    PIN_LED_EXHAUST,
    GRB
  >(
    exhaustLeds,
    LED_COUNT_EXHAUST
  );

  FastLED.addLeds<
    WS2812B,
    PIN_LED_HDD,
    GRB
  >(
    hddLeds,
    LED_COUNT_HDD
  );

  FastLED.addLeds<
    WS2812B,
    PIN_LED_CPU,
    GRB
  >(
    cpuLeds,
    LED_COUNT_CPU
  );

  globalBrightness =
    DEFAULT_BRIGHTNESS;

  effectBrightness = 255;

  applyBrightness();

  FastLED.clear(true);
}

void LedManager::setAll(
  const CRGB& color
)
{
  setIntake(color);
  setExhaust(color);
  setHdd(color);
  setCpu(color);
}

void LedManager::setHdd(
  const CRGB& color
)
{
  fill_solid(
    hddLeds,
    LED_COUNT_HDD,
    color
  );
}

void LedManager::setIntake(
  const CRGB& color
)
{
  fill_solid(
    intakeLeds,
    LED_COUNT_INTAKE,
    color
  );
}

void LedManager::setExhaust(
  const CRGB& color
)
{
  fill_solid(
    exhaustLeds,
    LED_COUNT_EXHAUST,
    color
  );
}

void LedManager::setCpu(
  const CRGB& color
)
{
  fill_solid(
    cpuLeds,
    LED_COUNT_CPU,
    color
  );
}

void LedManager::show()
{
  applyBrightness();
  FastLED.show();
}

void LedManager::clear()
{
  FastLED.clear();
  FastLED.show();
}

void LedManager::setBrightness(
  uint8_t brightness
)
{
  globalBrightness = brightness;
  applyBrightness();
}

uint8_t LedManager::
getBrightness() const
{
  return globalBrightness;
}

void LedManager::setEffectBrightness(
  uint8_t brightness
)
{
  effectBrightness = brightness;
  applyBrightness();
}

uint8_t LedManager::
getEffectBrightness() const
{
  return effectBrightness;
}

void LedManager::applyBrightness()
{
  const uint16_t combinedBrightness =
    (
      static_cast<uint16_t>(
        globalBrightness
      ) *
      static_cast<uint16_t>(
        effectBrightness
      )
    ) / 255U;

  FastLED.setBrightness(
    static_cast<uint8_t>(
      combinedBrightness
    )
  );
}

CRGB* LedManager::getHddLeds()
{
  return hddLeds;
}

CRGB* LedManager::getIntakeLeds()
{
  return intakeLeds;
}

CRGB* LedManager::getExhaustLeds()
{
  return exhaustLeds;
}

CRGB* LedManager::getCpuLeds()
{
  return cpuLeds;
}
