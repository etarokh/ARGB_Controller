#include "StaticColorEffect.h"
#include "Config.h"
#include "SettingsStorage.h"

void StaticColorEffect::begin(
  LedManager* ledManager
)
{
  leds = ledManager;

  colorIndex =
    settingsStorage().
      getStaticColorIndex();

  if (colorIndex >= COLOR_COUNT)
  {
    colorIndex = 0;
  }

  applyCurrentColor();

  needsRender = true;
}

void StaticColorEffect::update()
{
  if (
    leds == nullptr ||
    !needsRender
  )
  {
    return;
  }

  leds->setAll(
    effectColor
  );

  leds->show();

  needsRender = false;
}

void StaticColorEffect::reset()
{
  needsRender = true;
}

void StaticColorEffect::setColor(
  const CRGB& color
)
{
  effectColor = color;

  needsRender = true;
}

void StaticColorEffect::nextColor()
{
  colorIndex++;

  if (colorIndex >= COLOR_COUNT)
  {
    colorIndex = 0;
  }

  applyCurrentColor();

  settingsStorage().
    setStaticColorIndex(
      colorIndex
    );

  needsRender = true;
}

void StaticColorEffect::setColorIndex(
  uint8_t index
)
{
  if (index >= COLOR_COUNT)
  {
    index = 0;
  }

  colorIndex = index;

  applyCurrentColor();

  needsRender = true;
}

uint8_t StaticColorEffect::
getColorIndex() const
{
  return colorIndex;
}

const char*
StaticColorEffect::getColorName() const
{
  static const char*
    names[COLOR_COUNT] =
  {
    "White",
    "Warm White",
    "Cool White",
    "Red",
    "Dark Red",
    "Coral",
    "Orange",
    "Amber",
    "Gold",
    "Yellow",
    "Lime",
    "Chartreuse",
    "Green",
    "Emerald",
    "Mint",
    "Turquoise",
    "Cyan",
    "Aqua",
    "Sky Blue",
    "Deep Sky Blue",
    "Blue",
    "Royal Blue",
    "Indigo",
    "Violet",
    "Purple",
    "Lavender",
    "Magenta",
    "Pink",
    "Hot Pink",
    "Rose"
  };

  return names[colorIndex];
}

void StaticColorEffect::applyCurrentColor()
{
  static const CRGB
    colors[COLOR_COUNT] =
  {
    CRGB(255, 255, 255), // White
    CRGB(255, 180, 110), // Warm White
    CRGB(190, 220, 255), // Cool White

    CRGB(255, 0, 0),     // Red
    CRGB(120, 0, 0),     // Dark Red
    CRGB(255, 80, 65),   // Coral
    CRGB(255, 80, 0),    // Orange
    CRGB(255, 150, 0),   // Amber
    CRGB(255, 200, 40),  // Gold
    CRGB(255, 255, 0),   // Yellow

    CRGB(150, 255, 0),   // Lime
    CRGB(100, 255, 0),   // Chartreuse
    CRGB(0, 255, 0),     // Green
    CRGB(0, 200, 90),    // Emerald
    CRGB(80, 255, 170),  // Mint
    CRGB(0, 220, 180),   // Turquoise

    CRGB(0, 255, 255),   // Cyan
    CRGB(0, 180, 255),   // Aqua
    CRGB(70, 170, 255),  // Sky Blue
    CRGB(0, 110, 255),   // Deep Sky Blue
    CRGB(0, 0, 255),     // Blue
    CRGB(65, 105, 255),  // Royal Blue
    CRGB(75, 0, 130),    // Indigo

    CRGB(130, 40, 255),  // Violet
    CRGB(128, 0, 255),   // Purple
    CRGB(190, 140, 255), // Lavender
    CRGB(255, 0, 255),   // Magenta
    CRGB(255, 80, 170),  // Pink
    CRGB(255, 20, 147),  // Hot Pink
    CRGB(255, 45, 95)    // Rose
  };

  effectColor =
    colors[colorIndex];
}
