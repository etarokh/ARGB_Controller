#include "NebulaEffect.h"

void NebulaEffect::nextPattern()
{
  patternIndex =
    (patternIndex + 1) % PATTERN_COUNT;

  Serial.print("Nebula pattern: ");
  Serial.println(getPatternName());
}

uint8_t NebulaEffect::getPattern() const
{
  return patternIndex;
}

const char* NebulaEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "Purple Nebula";

    case 1:
      return "Blue Nebula";

    case 2:
      return "Pink Nebula";

    case 3:
      return "Deep Space";

    default:
      return "Unknown";
  }
}

void NebulaEffect::renderFrame()
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
      const uint8_t cloudOne =
        sin8(
          baseHue * 2 +
          group * 49 +
          ledIndex * 19
        );

      const uint8_t cloudTwo =
        cos8(
          baseHue +
          group * 73 +
          ledIndex * 27
        );

      const uint8_t cloud =
        static_cast<uint8_t>(
          (
            static_cast<uint16_t>(cloudOne) +
            static_cast<uint16_t>(cloudTwo)
          ) / 2
        );

      uint8_t hue;

      switch (patternIndex)
      {
        case 0:
          hue =
            static_cast<uint8_t>(
              178 + scale8(cloud, 52)
            );
          break;

        case 1:
          hue =
            static_cast<uint8_t>(
              135 + scale8(cloud, 48)
            );
          break;

        case 2:
          hue =
            static_cast<uint8_t>(
              205 + scale8(cloud, 42)
            );
          break;

        case 3:
        default:
          hue =
            static_cast<uint8_t>(
              155 + scale8(cloud, 70)
            );
          break;
      }

      const uint8_t brightness =
        static_cast<uint8_t>(
          75 + scale8(cloud, 180)
        );

      setGroupPixel(
        group,
        ledIndex,
        CHSV(
          hue,
          190,
          brightness
        )
      );
    }
  }
}
