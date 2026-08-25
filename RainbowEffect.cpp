#include "RainbowEffect.h"

void RainbowEffect::nextPattern()
{
  patternIndex++;

  if (patternIndex >= PATTERN_COUNT)
  {
    patternIndex = 0;
  }

  Serial.print(
    "Rainbow pattern: "
  );

  Serial.println(
    getPatternName()
  );
}

uint8_t RainbowEffect::getPattern() const
{
  return patternIndex;
}

const char*
RainbowEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "Synced";

    case 1:
      return "Split Rainbow";

    default:
      return "Unknown";
  }
}

void RainbowEffect::renderFrame()
{
  if (leds == nullptr)
  {
    return;
  }

  if (patternIndex == 0)
  {
    // Pattern 0: all LED groups use the same hue.
    const CRGB syncedColor =
      CHSV(
        baseHue,
        255,
        255
      );

    leds->setHdd(
      syncedColor
    );

    leds->setIntake(
      syncedColor
    );

    leds->setExhaust(
      syncedColor
    );

    leds->setCpu(
      syncedColor
    );
  }
  else
  {
    // Pattern 1: each LED group is offset by
    // one quarter of the hue wheel.
    const CRGB hddColor =
      CHSV(
        baseHue,
        255,
        255
      );

    const CRGB intakeColor =
      CHSV(
        baseHue + 64,
        255,
        255
      );

    const CRGB exhaustColor =
      CHSV(
        baseHue + 128,
        255,
        255
      );

    const CRGB cpuColor =
      CHSV(
        baseHue + 192,
        255,
        255
      );

    leds->setHdd(
      hddColor
    );

    leds->setIntake(
      intakeColor
    );

    leds->setExhaust(
      exhaustColor
    );

    leds->setCpu(
      cpuColor
    );
  }

  // Brightness is controlled by BrightnessManager.
  // Do not override effectBrightness here.
}
