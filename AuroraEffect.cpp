#include "AuroraEffect.h"
#include "SpeedCurve.h"
#include "Config.h"

void AuroraEffect::begin(
  LedManager* ledManager
)
{
  leds = ledManager;

  animationPhase = 0;
  palettePosition = 0;
  lastUpdateTime = 0;

  setSpeed(50);
}

void AuroraEffect::setPaletteManager(
  PaletteManager* paletteManager
)
{
  palette = paletteManager;
}

void AuroraEffect::update()
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

  animationPhase += 2;

  palettePosition++;

  render();
}

void AuroraEffect::reset()
{
  animationPhase = 0;
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

void AuroraEffect::setColor(
  const CRGB& color
)
{
  /*
    Aurora uses PaletteManager,
    so a single fixed color is ignored.
  */

  (void)color;
}

void AuroraEffect::setSpeed(
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

void AuroraEffect::nextPattern()
{
  patternIndex =
    (patternIndex + 1) % PATTERN_COUNT;

  Serial.print("Aurora pattern: ");
  Serial.println(getPatternName());
}

uint8_t AuroraEffect::getPattern() const
{
  return patternIndex;
}

const char* AuroraEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "Flowing Aurora";

    case 1:
      return "Synced Aurora";

    default:
      return "Unknown";
  }
}

uint8_t AuroraEffect::getSpeed() const
{
  return speed;
}

void AuroraEffect::updateIntervalFromSpeed()
{
  updateInterval =
    SpeedCurve::interval(
      speed,
      SpeedProfile::Continuous
    );
}

uint8_t AuroraEffect::calculateBrightness(
  uint8_t phaseOffset
) const
{
  uint8_t wavePosition =
    animationPhase + phaseOffset;

  /*
    sin8 returns a smooth value
    between 0 and 255.

    We limit brightness to 70..255
    so no group becomes fully dark.
  */

  return map(
    sin8(wavePosition),
    0,
    255,
    70,
    255
  );
}

void AuroraEffect::render()
{
  if (leds == nullptr)
  {
    return;
  }

  if (palette == nullptr)
  {
    return;
  }

  if (patternIndex == 1)
  {
    const uint8_t syncedBrightness =
      calculateBrightness(0);

    const CRGB syncedColor =
      palette->getColor(
        palettePosition,
        syncedBrightness
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
    const uint8_t hddBrightness =
      calculateBrightness(0);

    const uint8_t intakeBrightness =
      calculateBrightness(48);

    const uint8_t exhaustBrightness =
      calculateBrightness(96);

    const uint8_t cpuBrightness =
      calculateBrightness(144);

    const CRGB hddColor =
      palette->getColor(
        palettePosition,
        hddBrightness
      );

    const CRGB intakeColor =
      palette->getColor(
        palettePosition + 28,
        intakeBrightness
      );

    const CRGB exhaustColor =
      palette->getColor(
        palettePosition + 56,
        exhaustBrightness
      );

    const CRGB cpuColor =
      palette->getColor(
        palettePosition + 84,
        cpuBrightness
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

  leds->show();
}