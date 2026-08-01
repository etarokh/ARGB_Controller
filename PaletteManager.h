#pragma once

#include <Arduino.h>
#include <FastLED.h>

enum class DecorativePaletteType : uint8_t
{
  ClassicRGB = 0,
  Ocean,
  Forest,
  Fire,
  Lava,
  Galaxy,
  Ice,
  Sunset,
  Neon,
  Monochrome
};

class PaletteManager
{
public:
  void begin();

  void setPalette(
    DecorativePaletteType paletteType
  );

  DecorativePaletteType getPaletteType() const;

  void nextPalette();

  void previousPalette();

  void setReversed(bool reversed);

  bool isReversed() const;

  void toggleReversed();

  CRGB getColor(
    uint8_t position,
    uint8_t brightness = 255,
    TBlendType blendType = LINEARBLEND
  ) const;

  CRGB getIndexedColor(
    uint8_t colorIndex,
    uint8_t brightness = 255
  ) const;

  const CRGBPalette16& getPalette() const;

  const char* getPaletteName() const;

  static constexpr uint8_t getPaletteCount()
  {
    return 10;
  }

private:
  void loadPalette();

  static CRGBPalette16 createClassicRGBPalette();

  static CRGBPalette16 createOceanPalette();

  static CRGBPalette16 createForestPalette();

  static CRGBPalette16 createFirePalette();

  static CRGBPalette16 createLavaPalette();

  static CRGBPalette16 createGalaxyPalette();

  static CRGBPalette16 createIcePalette();

  static CRGBPalette16 createSunsetPalette();

  static CRGBPalette16 createNeonPalette();

  static CRGBPalette16 createMonochromePalette();

  DecorativePaletteType currentPaletteType =
    DecorativePaletteType::ClassicRGB;

  CRGBPalette16 currentPalette;

  bool reversed = false;
};