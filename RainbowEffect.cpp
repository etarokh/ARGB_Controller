#include "RainbowEffect.h"

void RainbowEffect::renderFrame()
{
  if (leds == nullptr)
  {
    return;
  }

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

  // Brightness is controlled by BrightnessManager.
  // Do not override effectBrightness here.
}
