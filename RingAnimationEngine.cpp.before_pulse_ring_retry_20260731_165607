#include "RingAnimationEngine.h"

void RingAnimationEngine::begin()
{
  reset();
}

void RingAnimationEngine::reset()
{
  phase = 0;

  progress = 0;

  hue = 0;

  filling = true;

  lastUpdateTime = 0;
}

void RingAnimationEngine::setPattern(
  RingAnimationPattern value
)
{
  if (pattern == value)
  {
    return;
  }

  pattern = value;

  reset();
}

void RingAnimationEngine::setUpdateInterval(
  unsigned long intervalMs
)
{
  updateIntervalMs = intervalMs;
}

bool RingAnimationEngine::update()
{
  const unsigned long currentTime =
    millis();

  if (
    currentTime - lastUpdateTime <
      updateIntervalMs
  )
  {
    return false;
  }

  lastUpdateTime = currentTime;

  switch (pattern)
  {
    case RingAnimationPattern::Spinner:
    case RingAnimationPattern::DualSpinner:
    case RingAnimationPattern::TripleSpinner:
    case RingAnimationPattern::KnightRider:
      updateSpinner();
      break;

    case RingAnimationPattern::HalfRing:
    case RingAnimationPattern::Alternate:
    case RingAnimationPattern::PulseRing:
      updateSpinner();
      break;

    case RingAnimationPattern::Fill:
    case RingAnimationPattern::Expand:
    case RingAnimationPattern::Contract:
    default:
      updateFill();
      break;
  }

  return true;
}

void RingAnimationEngine::updateFill()
{
  if (filling)
  {
    if (progress < LEDS_PER_RING)
    {
      progress++;
    }
    else
    {
      filling = false;
    }
  }
  else
  {
    if (progress > 0)
    {
      progress--;
    }
    else
    {
      filling = true;

      hue += 17;
    }
  }
}

void RingAnimationEngine::updateSpinner()
{
  phase++;

  if (phase >= LEDS_PER_RING)
  {
    phase = 0;

    hue += 17;
  }
}

void RingAnimationEngine::renderGroup(
  CRGB* leds,
  uint16_t ledCount
) const
{
  if (leds == nullptr)
  {
    return;
  }

  switch (pattern)
  {
    case RingAnimationPattern::Spinner:
      renderSpinner(
        leds,
        ledCount,
        1
      );
      break;

    case RingAnimationPattern::DualSpinner:
      renderSpinner(
        leds,
        ledCount,
        2
      );
      break;

    case RingAnimationPattern::TripleSpinner:
      renderSpinner(
        leds,
        ledCount,
        3
      );
      break;

    case RingAnimationPattern::KnightRider:
      renderKnightRider(
        leds,
        ledCount
      );
      break;

    case RingAnimationPattern::Expand:
      renderRadial(
        leds,
        ledCount,
        true
      );
      break;

    case RingAnimationPattern::Contract:
      renderRadial(
        leds,
        ledCount,
        false
      );
      break;

    case RingAnimationPattern::HalfRing:
      renderHalfRing(
        leds,
        ledCount
      );
      break;

    case RingAnimationPattern::Alternate:
      renderAlternate(
        leds,
        ledCount
      );
      break;

    case RingAnimationPattern::PulseRing:
      renderPulseRing(
        leds,
        ledCount
      );
      break;

    case RingAnimationPattern::Fill:
    default:
      renderFill(
        leds,
        ledCount
      );
      break;
  }
}

void RingAnimationEngine::renderFill(
  CRGB* leds,
  uint16_t ledCount
) const
{
  const CRGB color =
    getCurrentColor();

  for (
    uint16_t index = 0;
    index < ledCount;
    index++
  )
  {
    const uint8_t ledPosition =
      index % LEDS_PER_RING;

    if (ledPosition < progress)
    {
      leds[index] = color;
    }
    else
    {
      leds[index] = CRGB::Black;
    }
  }
}

void RingAnimationEngine::renderSpinner(
  CRGB* leds,
  uint16_t ledCount,
  uint8_t spinnerCount
) const
{
  const CRGB color =
    getCurrentColor();

  clearGroup(
    leds,
    ledCount
  );

  if (spinnerCount < 1)
  {
    spinnerCount = 1;
  }

  for (
    uint8_t spinnerIndex = 0;
    spinnerIndex < spinnerCount;
    spinnerIndex++
  )
  {
    const uint8_t position =
      (
        phase +
        (
          spinnerIndex *
          LEDS_PER_RING
        ) /
        spinnerCount
      ) %
      LEDS_PER_RING;

    drawPixel(
      leds,
      ledCount,
      position,
      color
    );
  }
}

void RingAnimationEngine::renderKnightRider(
  CRGB* leds,
  uint16_t ledCount
) const
{
  const CRGB color =
    getCurrentColor();

  const uint8_t cycleLength =
    (
      LEDS_PER_RING -
      1
    ) *
    2;

  const uint8_t cyclePhase =
    phase %
    cycleLength;

  const uint8_t headPosition =
    cyclePhase < LEDS_PER_RING
      ? cyclePhase
      : cycleLength - cyclePhase;

  for (
    uint16_t index = 0;
    index < ledCount;
    index++
  )
  {
    const uint8_t ledPosition =
      index % LEDS_PER_RING;

    if (ledPosition == headPosition)
    {
      leds[index] = color;
    }
    else if (
      ledPosition + 1 == headPosition ||
      ledPosition == headPosition + 1
    )
    {
      leds[index] = color;

      leds[index].fadeToBlackBy(
        160
      );
    }
    else
    {
      leds[index] = CRGB::Black;
    }
  }
}

void RingAnimationEngine::renderRadial(
  CRGB* leds,
  uint16_t ledCount,
  bool expanding
) const
{
  const CRGB color =
    getCurrentColor();

  const uint8_t clampedProgress =
    progress > 4
      ? 4
      : progress;

  const uint8_t distanceLimit =
    expanding
      ? clampedProgress
      : 4 - clampedProgress;

  for (
    uint16_t index = 0;
    index < ledCount;
    index++
  )
  {
    const uint8_t ledPosition =
      index % LEDS_PER_RING;

    const uint8_t clockwiseDistance =
      (
        ledPosition +
        LEDS_PER_RING -
        3
      ) %
      LEDS_PER_RING;

    const uint8_t counterClockwiseDistance =
      (
        3 +
        LEDS_PER_RING -
        ledPosition
      ) %
      LEDS_PER_RING;

    const uint8_t shortestDistance =
      min(
        clockwiseDistance,
        counterClockwiseDistance
      );

    const bool pixelOn =
      expanding
        ? shortestDistance < distanceLimit
        : shortestDistance >= distanceLimit;

    leds[index] =
      pixelOn
        ? color
        : CRGB::Black;
  }
}

void RingAnimationEngine::renderHalfRing(
  CRGB* leds,
  uint16_t ledCount
) const
{
  const CRGB color =
    getCurrentColor();

  for (
    uint16_t index = 0;
    index < ledCount;
    index++
  )
  {
    const uint8_t ledPosition =
      index % LEDS_PER_RING;

    const uint8_t relativePosition =
      (
        ledPosition +
        LEDS_PER_RING -
        phase
      ) %
      LEDS_PER_RING;

    if (
      relativePosition <
      LEDS_PER_RING / 2
    )
    {
      leds[index] = color;
    }
    else
    {
      leds[index] = CRGB::Black;
    }
  }
}

void RingAnimationEngine::renderAlternate(
  CRGB* leds,
  uint16_t ledCount
) const
{
  const CRGB primaryColor =
    getCurrentColor();

  const CRGB secondaryColor =
    CHSV(
      hue + 128,
      255,
      255
    );

  const bool invert =
    phase % 2 != 0;

  for (
    uint16_t index = 0;
    index < ledCount;
    index++
  )
  {
    const uint8_t ledPosition =
      index % LEDS_PER_RING;

    const bool usePrimary =
      (
        ledPosition % 2 == 0
      ) != invert;

    leds[index] =
      usePrimary
        ? primaryColor
        : secondaryColor;
  }
}

void RingAnimationEngine::renderPulseRing(
  CRGB* leds,
  uint16_t ledCount
) const
{
  CRGB color =
    getCurrentColor();

  const uint8_t brightness =
    sin8(
      phase * 32
    );

  color.nscale8_video(
    brightness
  );

  for (
    uint16_t index = 0;
    index < ledCount;
    index++
  )
  {
    leds[index] = color;
  }
}


void RingAnimationEngine::clearGroup(
  CRGB* leds,
  uint16_t ledCount
) const
{
  for (
    uint16_t i = 0;
    i < ledCount;
    i++
  )
  {
    leds[i] = CRGB::Black;
  }
}

void RingAnimationEngine::fillGroup(
  CRGB* leds,
  uint16_t ledCount,
  const CRGB& color
) const
{
  for (
    uint16_t i = 0;
    i < ledCount;
    i++
  )
  {
    leds[i] = color;
  }
}

void RingAnimationEngine::drawPixel(
  CRGB* leds,
  uint16_t ledCount,
  uint8_t ringPosition,
  const CRGB& color
) const
{
  ringPosition %= LEDS_PER_RING;

  for (
    uint16_t i = 0;
    i < ledCount;
    i++
  )
  {
    if (
      (i % LEDS_PER_RING) ==
      ringPosition
    )
    {
      leds[i] = color;
    }
  }
}


template<typename Func>
void RingAnimationEngine::forEachRingPixel(
  CRGB* leds,
  uint16_t ledCount,
  Func fn
) const
{
  for (
    uint16_t i = 0;
    i < ledCount;
    i++
  )
  {
    fn(
      i,
      i % LEDS_PER_RING
    );
  }
}

CRGB RingAnimationEngine::getCurrentColor() const
{
  return CHSV(
    hue,
    255,
    255
  );
}
