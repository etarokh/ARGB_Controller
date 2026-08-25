#include "SpectrumWaveEffect.h"

void SpectrumWaveEffect::nextPattern()
{
  patternIndex++;

  if (patternIndex >= PATTERN_COUNT)
  {
    patternIndex = 0;
  }

  Serial.print(
    "Spectrum Wave pattern: "
  );

  Serial.println(
    getPatternName()
  );
}

uint8_t SpectrumWaveEffect::
getPattern() const
{
  return patternIndex;
}

const char*
SpectrumWaveEffect::getPatternName() const
{
  switch (patternIndex)
  {
    case 0:
      return "Forward Flow";

    case 1:
      return "Reverse Flow";

    case 2:
      return "Rotating Spectrum";

    case 3:
      return "Diagonal Flow";

    case 4:
      return "Synced Spectrum";

    default:
      return "Unknown";
  }
}

void SpectrumWaveEffect::renderFrame()
{
  if (leds == nullptr)
  {
    return;
  }

  for (
    uint8_t groupIndex = 0;
    groupIndex < GROUP_COUNT;
    groupIndex++
  )
  {
    const uint16_t ledCount =
      getGroupLedCount(
        groupIndex
      );

    for (
      uint16_t ledIndex = 0;
      ledIndex < ledCount;
      ledIndex++
    )
    {
      const uint8_t hueOffset =
        calculateHueOffset(
          groupIndex,
          ledIndex
        );

      setGroupPixel(
        groupIndex,
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

uint8_t SpectrumWaveEffect::
calculateHueOffset(
  uint8_t groupIndex,
  uint16_t ledIndex
) const
{
  switch (patternIndex)
  {
    case 0:
      // Continuous forward flow:
      // HDD -> CPU -> Intake -> Exhaust
      return static_cast<uint8_t>(
        (
          groupIndex * 64
        ) +
        (
          ledIndex * 12
        )
      );

    case 1:
      // Reverse group and pixel direction.
      return static_cast<uint8_t>(
        (
          (GROUP_COUNT - 1 - groupIndex) * 64
        ) +
        (
          (
            getGroupLedCount(groupIndex) -
            1 -
            ledIndex
          ) * 12
        )
      );

    case 2:
      // Each group is one quarter of a rotating ring.
      return static_cast<uint8_t>(
        (
          groupIndex * 64
        ) +
        (
          ledIndex * 32
        )
      );

    case 3:
      // Diagonal phase offset between groups.
      return static_cast<uint8_t>(
        (
          groupIndex * 37
        ) +
        (
          ledIndex * 21
        )
      );

    case 4:
      // Synced Spectrum:
      // All LED groups use the same phase.
      // The group index is intentionally ignored.
      return static_cast<uint8_t>(
        ledIndex * 12
      );

    default:
      return 0;
  }
}
