#include "RowLedSyncEffect.h"
#include "SpeedCurve.h"
#include "Config.h"

void RowLedSyncEffect::begin(
  LedManager* ledManager
)
{
  leds = ledManager;

  animationEngine.begin();

  setSpeed(50);

  reset();
}

void RowLedSyncEffect::update()
{
  if (leds == nullptr)
  {
    return;
  }

  if (!animationEngine.update())
  {
    return;
  }

  render();
}

void RowLedSyncEffect::reset()
{
  animationEngine.setPattern(
    getEnginePattern()
  );

  animationEngine.reset();

  render();
}

void RowLedSyncEffect::setColor(
  const CRGB& color
)
{
  (void) color;
}

void RowLedSyncEffect::setSpeed(
  uint8_t speedPercent
)
{
  speed = constrain(
    speedPercent,
    1,
    100
  );

  updateInterval();
}

uint8_t RowLedSyncEffect::getSpeed() const
{
  return speed;
}

void RowLedSyncEffect::nextPattern()
{
  patternIndex++;

  if (patternIndex >= PATTERN_COUNT)
  {
    patternIndex = 0;
  }

  animationEngine.setPattern(
    getEnginePattern()
  );

  render();

  Serial.print(
    "Row LED Sync pattern: "
  );

  Serial.println(
    getPatternName()
  );
}

void RowLedSyncEffect::setPattern(
  uint8_t pattern
)
{
  if (pattern >= PATTERN_COUNT)
  {
    pattern = 0;
  }

  patternIndex = pattern;

  animationEngine.setPattern(
    getEnginePattern()
  );

  render();
}

uint8_t RowLedSyncEffect::getPattern() const
{
  return patternIndex;
}

const char*
RowLedSyncEffect::getPatternName() const
{
  switch (getEnginePattern())
  {
    case RingAnimationPattern::Fill:
      return "Fill";

    case RingAnimationPattern::Spinner:
      return "Spinner";

    case RingAnimationPattern::DualSpinner:
      return "Dual Spinner";

    case RingAnimationPattern::TripleSpinner:
      return "Triple Spinner";

    case RingAnimationPattern::KnightRider:
      return "Knight Rider";

    case RingAnimationPattern::Expand:
      return "Expand";

    case RingAnimationPattern::Contract:
      return "Contract";

    case RingAnimationPattern::HalfRing:
      return "Half Ring";

    case RingAnimationPattern::Alternate:
      return "Alternate";

    case RingAnimationPattern::PulseRing:
      return "Pulse Ring";

    default:
      return "Unknown";
  }
}

void RowLedSyncEffect::render()
{
  if (leds == nullptr)
  {
    return;
  }

  animationEngine.renderGroup(
    leds->getHddLeds(),
    LED_COUNT_HDD
  );

  animationEngine.renderGroup(
    leds->getIntakeLeds(),
    LED_COUNT_INTAKE
  );

  animationEngine.renderGroup(
    leds->getExhaustLeds(),
    LED_COUNT_EXHAUST
  );

  animationEngine.renderGroup(
    leds->getCpuLeds(),
    LED_COUNT_CPU
  );

  leds->setEffectBrightness(255);

  leds->show();
}

void RowLedSyncEffect::updateInterval()
{
  updateIntervalMs =
    SpeedCurve::interval(
      speed,
      SpeedProfile::RowLedSync
    );

  animationEngine.setUpdateInterval(
    updateIntervalMs
  );
}

RingAnimationPattern
RowLedSyncEffect::getEnginePattern() const
{
  return static_cast<RingAnimationPattern>(
    patternIndex
  );
}
