#include "PaletteManager.h"

namespace
{
  uint8_t reversePosition(
    uint8_t position
  )
  {
    return static_cast<uint8_t>(
      255 - position
    );
  }
}

void PaletteManager::begin()
{
  currentPaletteType =
    DecorativePaletteType::ClassicRGB;

  reversed = false;

  loadPalette();

  Serial.print("Palette initialized: ");
  Serial.println(getPaletteName());
}

void PaletteManager::setPalette(
  DecorativePaletteType paletteType
)
{
  if (
    static_cast<uint8_t>(paletteType) >=
    getPaletteCount()
  )
  {
    return;
  }

  if (currentPaletteType == paletteType)
  {
    return;
  }

  currentPaletteType = paletteType;

  loadPalette();

  Serial.print("Palette changed: ");
  Serial.println(getPaletteName());
}

DecorativePaletteType
PaletteManager::getPaletteType() const
{
  return currentPaletteType;
}

void PaletteManager::nextPalette()
{
  uint8_t nextIndex =
    static_cast<uint8_t>(
      currentPaletteType
    );

  nextIndex++;

  if (nextIndex >= getPaletteCount())
  {
    nextIndex = 0;
  }

  setPalette(
    static_cast<DecorativePaletteType>(
      nextIndex
    )
  );
}

void PaletteManager::previousPalette()
{
  uint8_t currentIndex =
    static_cast<uint8_t>(
      currentPaletteType
    );

  uint8_t previousIndex;

  if (currentIndex == 0)
  {
    previousIndex =
      getPaletteCount() - 1;
  }
  else
  {
    previousIndex =
      currentIndex - 1;
  }

  setPalette(
    static_cast<DecorativePaletteType>(
      previousIndex
    )
  );
}

void PaletteManager::setReversed(
  bool shouldReverse
)
{
  if (reversed == shouldReverse)
  {
    return;
  }

  reversed = shouldReverse;

  Serial.print("Palette direction: ");

  if (reversed)
  {
    Serial.println("REVERSED");
  }
  else
  {
    Serial.println("NORMAL");
  }
}

bool PaletteManager::isReversed() const
{
  return reversed;
}

void PaletteManager::toggleReversed()
{
  setReversed(!reversed);
}

CRGB PaletteManager::getColor(
  uint8_t position,
  uint8_t brightness,
  TBlendType blendType
) const
{
  uint8_t finalPosition = position;

  if (reversed)
  {
    finalPosition =
      reversePosition(position);
  }

  return ColorFromPalette(
    currentPalette,
    finalPosition,
    brightness,
    blendType
  );
}

CRGB PaletteManager::getIndexedColor(
  uint8_t colorIndex,
  uint8_t brightness
) const
{
  /*
    The 16-color FastLED palette uses
    positions from 0 to 255.

    Multiplying by 16 selects one of
    the 16 main palette entries.
  */

  uint8_t position =
    static_cast<uint8_t>(
      (colorIndex & 0x0F) * 16
    );

  return getColor(
    position,
    brightness,
    NOBLEND
  );
}

const CRGBPalette16&
PaletteManager::getPalette() const
{
  return currentPalette;
}

const char*
PaletteManager::getPaletteName() const
{
  switch (currentPaletteType)
  {
    case DecorativePaletteType::ClassicRGB:
      return "CLASSIC RGB";

    case DecorativePaletteType::Ocean:
      return "OCEAN";

    case DecorativePaletteType::Forest:
      return "FOREST";

    case DecorativePaletteType::Fire:
      return "FIRE";

    case DecorativePaletteType::Lava:
      return "LAVA";

    case DecorativePaletteType::Galaxy:
      return "GALAXY";

    case DecorativePaletteType::Ice:
      return "ICE";

    case DecorativePaletteType::Sunset:
      return "SUNSET";

    case DecorativePaletteType::Neon:
      return "NEON";

    case DecorativePaletteType::Monochrome:
      return "MONOCHROME";

    default:
      return "UNKNOWN";
  }
}

void PaletteManager::loadPalette()
{
  switch (currentPaletteType)
  {
    case DecorativePaletteType::ClassicRGB:
      currentPalette =
        createClassicRGBPalette();
      break;

    case DecorativePaletteType::Ocean:
      currentPalette =
        createOceanPalette();
      break;

    case DecorativePaletteType::Forest:
      currentPalette =
        createForestPalette();
      break;

    case DecorativePaletteType::Fire:
      currentPalette =
        createFirePalette();
      break;

    case DecorativePaletteType::Lava:
      currentPalette =
        createLavaPalette();
      break;

    case DecorativePaletteType::Galaxy:
      currentPalette =
        createGalaxyPalette();
      break;

    case DecorativePaletteType::Ice:
      currentPalette =
        createIcePalette();
      break;

    case DecorativePaletteType::Sunset:
      currentPalette =
        createSunsetPalette();
      break;

    case DecorativePaletteType::Neon:
      currentPalette =
        createNeonPalette();
      break;

    case DecorativePaletteType::Monochrome:
      currentPalette =
        createMonochromePalette();
      break;

    default:
      currentPalette =
        createClassicRGBPalette();
      break;
  }
}

CRGBPalette16
PaletteManager::createClassicRGBPalette()
{
  return CRGBPalette16(
    CRGB::Red,
    CRGB::OrangeRed,
    CRGB::Orange,
    CRGB::Yellow,

    CRGB::Green,
    CRGB::SpringGreen,
    CRGB::Cyan,
    CRGB::DeepSkyBlue,

    CRGB::Blue,
    CRGB::Indigo,
    CRGB::Purple,
    CRGB::Magenta,

    CRGB::DeepPink,
    CRGB::Red,
    CRGB::Green,
    CRGB::Blue
  );
}

CRGBPalette16
PaletteManager::createOceanPalette()
{
  return CRGBPalette16(
    CRGB(0, 4, 20),
    CRGB(0, 12, 48),
    CRGB(0, 30, 90),
    CRGB(0, 55, 140),

    CRGB(0, 85, 190),
    CRGB(0, 120, 220),
    CRGB(0, 160, 230),
    CRGB(0, 200, 220),

    CRGB(0, 230, 200),
    CRGB(35, 255, 210),
    CRGB(90, 255, 225),
    CRGB(150, 255, 240),

    CRGB(90, 220, 255),
    CRGB(30, 155, 255),
    CRGB(0, 85, 190),
    CRGB(0, 25, 75)
  );
}

CRGBPalette16
PaletteManager::createForestPalette()
{
  return CRGBPalette16(
    CRGB(0, 8, 0),
    CRGB(0, 18, 2),
    CRGB(0, 32, 4),
    CRGB(0, 48, 6),

    CRGB(0, 70, 8),
    CRGB(0, 95, 12),
    CRGB(5, 125, 18),
    CRGB(10, 155, 25),

    CRGB(25, 185, 35),
    CRGB(50, 210, 50),
    CRGB(85, 225, 70),
    CRGB(125, 235, 90),

    CRGB(75, 180, 45),
    CRGB(30, 120, 20),
    CRGB(5, 65, 8),
    CRGB(0, 25, 2)
  );
}

CRGBPalette16
PaletteManager::createFirePalette()
{
  return CRGBPalette16(
    CRGB::Black,
    CRGB(30, 0, 0),
    CRGB(70, 0, 0),
    CRGB(110, 0, 0),

    CRGB(150, 5, 0),
    CRGB(190, 15, 0),
    CRGB(225, 35, 0),
    CRGB(255, 60, 0),

    CRGB(255, 90, 0),
    CRGB(255, 125, 0),
    CRGB(255, 160, 0),
    CRGB(255, 195, 0),

    CRGB(255, 220, 30),
    CRGB(255, 240, 90),
    CRGB(255, 250, 170),
    CRGB::White
  );
}

CRGBPalette16
PaletteManager::createLavaPalette()
{
  return CRGBPalette16(
    CRGB(8, 0, 0),
    CRGB(25, 0, 5),
    CRGB(50, 0, 12),
    CRGB(85, 0, 18),

    CRGB(120, 0, 20),
    CRGB(155, 0, 15),
    CRGB(190, 5, 5),
    CRGB(225, 20, 0),

    CRGB(255, 45, 0),
    CRGB(255, 75, 0),
    CRGB(255, 110, 0),
    CRGB(255, 145, 5),

    CRGB(230, 55, 15),
    CRGB(175, 5, 35),
    CRGB(100, 0, 35),
    CRGB(35, 0, 18)
  );
}

CRGBPalette16
PaletteManager::createGalaxyPalette()
{
  return CRGBPalette16(
    CRGB(3, 0, 15),
    CRGB(8, 0, 35),
    CRGB(15, 0, 65),
    CRGB(25, 0, 100),

    CRGB(40, 0, 140),
    CRGB(65, 0, 180),
    CRGB(90, 0, 215),
    CRGB(120, 0, 235),

    CRGB(150, 10, 255),
    CRGB(185, 30, 255),
    CRGB(220, 55, 245),
    CRGB(245, 80, 220),

    CRGB(160, 55, 255),
    CRGB(90, 35, 230),
    CRGB(35, 15, 150),
    CRGB(8, 2, 45)
  );
}

CRGBPalette16
PaletteManager::createIcePalette()
{
  return CRGBPalette16(
    CRGB(0, 8, 20),
    CRGB(0, 20, 45),
    CRGB(0, 40, 80),
    CRGB(0, 65, 115),

    CRGB(0, 90, 150),
    CRGB(0, 125, 185),
    CRGB(15, 160, 210),
    CRGB(45, 190, 230),

    CRGB(85, 215, 245),
    CRGB(125, 230, 255),
    CRGB(165, 240, 255),
    CRGB(200, 248, 255),

    CRGB(225, 252, 255),
    CRGB::White,
    CRGB(180, 235, 255),
    CRGB(70, 170, 225)
  );
}

CRGBPalette16
PaletteManager::createSunsetPalette()
{
  return CRGBPalette16(
    CRGB(40, 0, 20),
    CRGB(75, 0, 35),
    CRGB(110, 0, 50),
    CRGB(150, 5, 60),

    CRGB(190, 15, 65),
    CRGB(225, 30, 60),
    CRGB(255, 50, 45),
    CRGB(255, 75, 25),

    CRGB(255, 105, 15),
    CRGB(255, 140, 20),
    CRGB(255, 175, 40),
    CRGB(255, 205, 75),

    CRGB(245, 110, 100),
    CRGB(215, 55, 130),
    CRGB(150, 20, 125),
    CRGB(75, 5, 75)
  );
}

CRGBPalette16
PaletteManager::createNeonPalette()
{
  return CRGBPalette16(
    CRGB(255, 0, 80),
    CRGB(255, 0, 180),
    CRGB(210, 0, 255),
    CRGB(120, 0, 255),

    CRGB(20, 0, 255),
    CRGB(0, 100, 255),
    CRGB(0, 220, 255),
    CRGB(0, 255, 190),

    CRGB(0, 255, 70),
    CRGB(130, 255, 0),
    CRGB(230, 255, 0),
    CRGB(255, 190, 0),

    CRGB(255, 80, 0),
    CRGB(255, 0, 40),
    CRGB(255, 0, 150),
    CRGB(100, 0, 255)
  );
}

CRGBPalette16
PaletteManager::createMonochromePalette()
{
  return CRGBPalette16(
    CRGB(2, 2, 2),
    CRGB(6, 6, 6),
    CRGB(12, 12, 12),
    CRGB(20, 20, 20),

    CRGB(32, 32, 32),
    CRGB(48, 48, 48),
    CRGB(68, 68, 68),
    CRGB(92, 92, 92),

    CRGB(120, 120, 120),
    CRGB(150, 150, 150),
    CRGB(180, 180, 180),
    CRGB(205, 205, 205),

    CRGB(225, 225, 225),
    CRGB(240, 240, 240),
    CRGB(250, 250, 250),
    CRGB::White
  );
}