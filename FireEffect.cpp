#include "FireEffect.h"

void FireEffect::nextPattern()
{
  patternIndex =
    (patternIndex + 1) % PATTERN_COUNT;

  Serial.print("Fire pattern: ");
  Serial.println(getPatternName());
}

uint8_t FireEffect::getPattern() const
{
  return patternIndex;
}

const char* FireEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "Classic Fire";

    case 1:
      return "Blue Fire";

    case 2:
      return "Green Fire";

    case 3:
      return "Purple Fire";

    default:
      return "Unknown";
  }
}

void FireEffect::renderFrame()
{
  for (
    uint8_t group = 0;
    group < GROUP_COUNT;
    group++
  )
  {
    const uint16_t ledCount =
      getGroupLedCount(group);

    for (
      uint16_t ledIndex = 0;
      ledIndex < ledCount;
      ledIndex++
    )
    {
      const uint8_t flicker =
        random8(145, 255);

      const uint8_t pulse =
        sin8(
          baseHue * 4 +
          group * 41 +
          ledIndex * 31
        );

      const uint8_t brightness =
        static_cast<uint8_t>(
          (
            static_cast<uint16_t>(flicker) +
            static_cast<uint16_t>(pulse)
          ) / 2
        );

      uint8_t hue;

      switch (patternIndex)
      {
        case 0:
          hue =
            static_cast<uint8_t>(
              2 + random8(34)
            );
          break;

        case 1:
          hue =
            static_cast<uint8_t>(
              138 + random8(34)
            );
          break;

        case 2:
          hue =
            static_cast<uint8_t>(
              78 + random8(34)
            );
          break;

        case 3:
        default:
          hue =
            static_cast<uint8_t>(
              185 + random8(35)
            );
          break;
      }

      setGroupPixel(
        group,
        ledIndex,
        CHSV(
          hue,
          255,
          brightness
        )
      );
    }
  }
}
