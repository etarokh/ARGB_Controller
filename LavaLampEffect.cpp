#include "LavaLampEffect.h"

void LavaLampEffect::nextPattern()
{
  patternIndex =
    (patternIndex + 1) % PATTERN_COUNT;

  Serial.print("Lava Lamp pattern: ");
  Serial.println(getPatternName());
}

uint8_t LavaLampEffect::getPattern() const
{
  return patternIndex;
}

const char* LavaLampEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "Warm Lava";

    case 1:
      return "Ocean Lava";

    case 2:
      return "Galaxy Lava";

    case 3:
      return "Rainbow Lava";

    default:
      return "Unknown";
  }
}

void LavaLampEffect::renderFrame()
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
      const uint8_t phase =
        static_cast<uint8_t>(
          baseHue * 2 +
          group * 53 +
          ledIndex * 24
        );

      const uint8_t blob =
        sin8(phase);

      uint8_t hue;

      switch (patternIndex)
      {
        case 0:
          hue =
            static_cast<uint8_t>(
              2 + scale8(blob, 34)
            );
          break;

        case 1:
          hue =
            static_cast<uint8_t>(
              132 + scale8(blob, 45)
            );
          break;

        case 2:
          hue =
            static_cast<uint8_t>(
              175 + scale8(blob, 55)
            );
          break;

        case 3:
        default:
          hue =
            static_cast<uint8_t>(
              baseHue +
              group * 48 +
              ledIndex * 12
            );
          break;
      }

      const uint8_t brightness =
        static_cast<uint8_t>(
          105 + scale8(blob, 150)
        );

      setGroupPixel(
        group,
        ledIndex,
        CHSV(
          hue,
          235,
          brightness
        )
      );
    }
  }
}
