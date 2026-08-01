#include "PlasmaEffect.h"

void PlasmaEffect::nextPattern()
{
  patternIndex =
    (patternIndex + 1) % PATTERN_COUNT;

  Serial.print("Plasma pattern: ");
  Serial.println(getPatternName());
}

uint8_t PlasmaEffect::getPattern() const
{
  return patternIndex;
}

const char* PlasmaEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "Classic Plasma";

    case 1:
      return "Electric Plasma";

    case 2:
      return "Wide Plasma";

    case 3:
      return "Mirror Plasma";

    default:
      return "Unknown";
  }
}

void PlasmaEffect::renderFrame()
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
      const uint8_t waveOne =
        sin8(
          baseHue * 3 +
          group * 47 +
          ledIndex * 29
        );

      const uint8_t waveTwo =
        cos8(
          baseHue * 2 +
          group * 71 -
          ledIndex * 17
        );

      const uint8_t combined =
        static_cast<uint8_t>(
          (
            static_cast<uint16_t>(waveOne) +
            static_cast<uint16_t>(waveTwo)
          ) / 2
        );

      uint8_t hueOffset;

      switch (patternIndex)
      {
        case 0:
          hueOffset = combined;
          break;

        case 1:
          hueOffset =
            static_cast<uint8_t>(
              combined / 2 +
              group * 36
            );
          break;

        case 2:
          hueOffset =
            static_cast<uint8_t>(
              combined +
              ledIndex * 20
            );
          break;

        case 3:
        default:
        {
          const uint8_t mirroredGroup =
            group < 2
              ? group
              : GROUP_COUNT - 1 - group;

          hueOffset =
            static_cast<uint8_t>(
              combined +
              mirroredGroup * 64
            );
          break;
        }
      }

      setGroupPixel(
        group,
        ledIndex,
        hsvColor(
          hueOffset,
          255,
          255
        )
      );
    }
  }
}
