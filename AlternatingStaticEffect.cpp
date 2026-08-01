#include "AlternatingStaticEffect.h"

void AlternatingStaticEffect::begin(
  LedManager* ledManager
)
{
  leds = ledManager;

  patternIndex = 0;

  needsRender = true;
}

void AlternatingStaticEffect::update()
{
  if (
    leds == nullptr ||
    !needsRender
  )
  {
    return;
  }

  render();

  needsRender = false;
}

void AlternatingStaticEffect::reset()
{
  needsRender = true;
}

void AlternatingStaticEffect::setColor(
  const CRGB& color
)
{
  // This effect uses predefined color pairs.
  (void)color;
}

void AlternatingStaticEffect::setSpeed(
  uint8_t speedPercent
)
{
  speed = constrain(
    speedPercent,
    1,
    100
  );
}

uint8_t AlternatingStaticEffect::
getSpeed() const
{
  return speed;
}

void AlternatingStaticEffect::nextPattern()
{
  patternIndex++;

  if (patternIndex >= PATTERN_COUNT)
  {
    patternIndex = 0;
  }

  needsRender = true;

  Serial.print(
    "Alternating Static pattern: "
  );

  Serial.println(
    getPatternName()
  );
}

uint8_t AlternatingStaticEffect::
getPattern() const
{
  return patternIndex;
}

const char*
AlternatingStaticEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "White / Red";

    case 1:
      return "Red / White";

    case 2:
      return "White / Blue";

    case 3:
      return "Blue / White";

    case 4:
      return "Green / Purple";

    case 5:
      return "Purple / Green";

    case 6:
      return "Yellow / Blue";

    case 7:
      return "Blue / Yellow";

    case 8:
      return "Cyan / Pink";

    case 9:
      return "Pink / Cyan";

    case 10:
      return "Orange / White";

    case 11:
      return "White / Orange";

    default:
      return "Unknown";
  }
}

void AlternatingStaticEffect::render()
{
  if (leds == nullptr)
  {
    return;
  }

  const CRGB firstColor =
    getFirstColor();

  const CRGB secondColor =
    getSecondColor();

  // Alternating groups:
  // HDD and Intake use color one.
  // CPU and Exhaust use color two.
  leds->setHdd(
    firstColor
  );

  leds->setCpu(
    secondColor
  );

  leds->setIntake(
    firstColor
  );

  leds->setExhaust(
    secondColor
  );

  // Do not override effect/global brightness here.
  leds->show();
}

CRGB AlternatingStaticEffect::
getFirstColor() const
{
  switch (patternIndex)
  {
    case 0:
      return CRGB::White;

    case 1:
      return CRGB::Red;

    case 2:
      return CRGB::White;

    case 3:
      return CRGB::Blue;

    case 4:
      return CRGB::Green;

    case 5:
      return CRGB::Purple;

    case 6:
      return CRGB::Yellow;

    case 7:
      return CRGB::Blue;

    case 8:
      return CRGB::Cyan;

    case 9:
      return CRGB::HotPink;

    case 10:
      return CRGB::Orange;

    case 11:
      return CRGB::White;

    default:
      return CRGB::White;
  }
}

CRGB AlternatingStaticEffect::
getSecondColor() const
{
  switch (patternIndex)
  {
    case 0:
      return CRGB::Red;

    case 1:
      return CRGB::White;

    case 2:
      return CRGB::Blue;

    case 3:
      return CRGB::White;

    case 4:
      return CRGB::Purple;

    case 5:
      return CRGB::Green;

    case 6:
      return CRGB::Blue;

    case 7:
      return CRGB::Yellow;

    case 8:
      return CRGB::HotPink;

    case 9:
      return CRGB::Cyan;

    case 10:
      return CRGB::White;

    case 11:
      return CRGB::Orange;

    default:
      return CRGB::Red;
  }
}
