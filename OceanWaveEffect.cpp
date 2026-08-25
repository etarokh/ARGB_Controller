#include "OceanWaveEffect.h"
#include "SpeedCurve.h"
#include "Config.h"

void OceanWaveEffect::begin(
  LedManager* ledManager
)
{
  leds = ledManager;

  wavePosition = 0;
  waveDirection = 1;
  palettePosition = 0;
  lastUpdateTime = 0;

  setSpeed(50);
}

void OceanWaveEffect::setPaletteManager(
  PaletteManager* paletteManager
)
{
  palette = paletteManager;
}

void OceanWaveEffect::update()
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

  /*
    Move the wave back and forth
    through the full 0..255 range.
  */

  int16_t nextPosition =
    static_cast<int16_t>(
      wavePosition
    ) + waveDirection * 3;

  if (nextPosition >= 255)
  {
    nextPosition = 255;
    waveDirection = -1;
  }
  else if (nextPosition <= 0)
  {
    nextPosition = 0;
    waveDirection = 1;
  }

  wavePosition =
    static_cast<uint8_t>(
      nextPosition
    );

  palettePosition++;

  render();
}

void OceanWaveEffect::reset()
{
  wavePosition = 0;
  waveDirection = 1;
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

void OceanWaveEffect::setColor(
  const CRGB& color
)
{
  /*
    Ocean Wave uses PaletteManager,
    so a fixed color is ignored.
  */

  (void)color;
}

void OceanWaveEffect::setSpeed(
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

uint8_t OceanWaveEffect::getSpeed() const
{
  return speed;
}

void OceanWaveEffect::nextPattern()
{
  patternIndex =
    (patternIndex + 1) % PATTERN_COUNT;

  Serial.print("Ocean Wave pattern: ");
  Serial.println(getPatternName());
}

uint8_t OceanWaveEffect::getPattern() const
{
  return patternIndex;
}

const char* OceanWaveEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "Flowing Ocean";

    case 1:
      return "Synced Ocean";

    default:
      return "Unknown";
  }
}

void OceanWaveEffect::updateIntervalFromSpeed()
{
  updateInterval =
    SpeedCurve::interval(
      speed,
      SpeedProfile::Continuous
    );
}

uint8_t OceanWaveEffect::calculateWaveBrightness(
  uint8_t groupPosition
) const
{
  /*
    Calculate circular distance between
    the moving wave and each group.
  */

  uint8_t distance =
    abs8(
      static_cast<int8_t>(
        wavePosition - groupPosition
      )
    );

  /*
    Close to the wave center:
      brighter

    Far from the wave center:
      dimmer
  */

  uint8_t brightness =
    255 - qsub8(
      distance * 3,
      0
    );

  /*
    Keep every group slightly visible.
  */

  if (brightness < 35)
  {
    brightness = 35;
  }

  return brightness;
}

void OceanWaveEffect::render()
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
      calculateWaveBrightness(0);

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
      calculateWaveBrightness(0);

    const uint8_t intakeBrightness =
      calculateWaveBrightness(85);

    const uint8_t exhaustBrightness =
      calculateWaveBrightness(170);

    const uint8_t cpuBrightness =
      calculateWaveBrightness(255);

    const CRGB hddColor =
      palette->getColor(
        palettePosition,
        hddBrightness
      );

    const CRGB intakeColor =
      palette->getColor(
        palettePosition + 18,
        intakeBrightness
      );

    const CRGB exhaustColor =
      palette->getColor(
        palettePosition + 36,
        exhaustBrightness
      );

    const CRGB cpuColor =
      palette->getColor(
        palettePosition + 54,
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