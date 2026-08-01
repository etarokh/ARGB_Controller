#include "ColorCycleEffect.h"
#include "SpeedCurve.h"
#include "Config.h"

void ColorCycleEffect::begin(
  LedManager* ledManager
)
{
  leds = ledManager;

  palettePosition = 0;
  lastUpdateTime = 0;

  setSpeed(50);
}

void ColorCycleEffect::setPaletteManager(
  PaletteManager* paletteManager
)
{
  palette = paletteManager;
}

void ColorCycleEffect::update()
{
  if (leds == nullptr)
  {
    return;
  }

  if (palette == nullptr)
  {
    return;
  }

  unsigned long currentTime = millis();

  if (
    currentTime - lastUpdateTime <
    updateInterval
  )
  {
    return;
  }

  lastUpdateTime = currentTime;

  palettePosition++;

  render();
}

void ColorCycleEffect::reset()
{
  palettePosition = 0;
  lastUpdateTime = 0;

  if (leds == nullptr)
  {
    return;
  }

  leds->setEffectBrightness(
    255
  );

  render();
}

void ColorCycleEffect::setColor(
  const CRGB& color
)
{
  /*
    Color Cycle uses PaletteManager,
    therefore a fixed color is ignored.
  */

  (void)color;
}

void ColorCycleEffect::setSpeed(
  uint8_t speedPercent
)
{
  if (speedPercent < 1)
  {
    speedPercent = 1;
  }

  if (speedPercent > 100)
  {
    speedPercent = 100;
  }

  speed = speedPercent;

  updateIntervalFromSpeed();
}

uint8_t ColorCycleEffect::getSpeed() const
{
  return speed;
}

void ColorCycleEffect::updateIntervalFromSpeed()
{
  updateInterval =
    SpeedCurve::interval(
      speed,
      SpeedProfile::Continuous
    );
}

void ColorCycleEffect::render()
{
  if (leds == nullptr)
  {
    return;
  }

  if (palette == nullptr)
  {
    return;
  }

  /*
    The four groups are separated by
    one quarter of the palette.

    256 / 4 = 64
  */

  const uint8_t hddPosition =
    palettePosition;

  const uint8_t intakePosition =
    palettePosition + 64;

  const uint8_t exhaustPosition =
    palettePosition + 128;

  const uint8_t cpuPosition =
    palettePosition + 192;

  CRGB hddColor =
    palette->getColor(
      hddPosition
    );

  CRGB intakeColor =
    palette->getColor(
      intakePosition
    );

  CRGB exhaustColor =
    palette->getColor(
      exhaustPosition
    );

  CRGB cpuColor =
    palette->getColor(
      cpuPosition
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

  leds->show();
}