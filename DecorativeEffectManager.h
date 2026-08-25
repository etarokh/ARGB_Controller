#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "LedManager.h"
#include "DecorativeEffect.h"
#include "StaticColorEffect.h"
#include "BreathingEffect.h"
#include "RainbowEffect.h"
#include "SpectrumWaveEffect.h"
#include "TwinkleStarsEffect.h"
#include "GradientFlowEffect.h"
#include "LavaLampEffect.h"
#include "PlasmaEffect.h"
#include "FireEffect.h"
#include "NebulaEffect.h"
#include "RowLedSyncEffect.h"
#include "GroupFillWaveEffect.h"
#include "CometEffect.h"
#include "AlternatingStaticEffect.h"
#include "ColorCycleEffect.h"
#include "AuroraEffect.h"
#include "OceanWaveEffect.h"
#include "PaletteManager.h"

enum class DecorativeEffectType
{
  StaticColor,
  Rainbow,
  SpectrumWave,
  TwinkleStars,
  GradientFlow,
  LavaLamp,
  Plasma,
  Fire,
  Nebula,
  RowLedSync,
  GroupFillWave,
  Comet,
  AlternatingStatic,
  Breathing,
  ColorCycle,
  Aurora,
  OceanWave
};

class DecorativeEffectManager
{
public:
  void begin(
    LedManager* ledManager
  );

  void update();

  void setEnabled(
    bool enabled
  );

  bool isEnabled() const;

  void setEffect(
    DecorativeEffectType effect
  );

  DecorativeEffectType
  getEffect() const;

  void nextEffect();

  void nextStaticColor();

  void setStaticColorIndex(
    uint8_t index
  );

  uint8_t
  getStaticColorIndex() const;

  bool hasPattern() const;

  uint8_t getPattern() const;

  uint8_t getPatternCount() const;

  void setPattern(
    uint8_t pattern
  );

  void nextPattern();

  const char* getPatternName() const;

  const char* getStaticColorName() const;

  void previousEffect();

  const char* getEffectName() const;

  void setColor(
    const CRGB& color
  );

  void setPalette(
    DecorativePaletteType paletteType
  );

  DecorativePaletteType
  getPaletteType() const;

  void nextPalette();

  void previousPalette();

  void setPaletteReversed(
    bool reversed
  );

  void togglePaletteReversed();

  bool isPaletteReversed() const;

  const char* getPaletteName() const;

  void setSpeed(
    uint8_t speedPercent
  );

  uint8_t getSpeed() const;

  void increaseSpeed(
    uint8_t amount = 10
  );

  void decreaseSpeed(
    uint8_t amount = 10
  );

  void clear();

  void setAutoCycleEnabled(
    bool enabled
  );

  bool isAutoCycleEnabled() const;

  void setAutoCycleIntervalSeconds(
    uint16_t seconds
  );

  uint16_t getAutoCycleIntervalSeconds() const;

private:
  static constexpr uint8_t EFFECT_COUNT = 17;

  void updateAutoCycle();

  void advanceAutoCycle();

  bool autoCycleEnabled = false;

  uint16_t autoCycleIntervalSeconds = 60;

  unsigned long autoCycleLastChangeMs = 0;

  void selectCurrentEffect();

  void applySpeedToEffects();

  LedManager* leds = nullptr;

  bool enabled = false;

  uint8_t decorativeSpeed = 50;

  DecorativeEffectType currentEffectType =
    DecorativeEffectType::StaticColor;

  DecorativeEffect* currentEffect =
    nullptr;

  PaletteManager paletteManager;

  StaticColorEffect staticColorEffect;

  BreathingEffect breathingEffect;

  RainbowEffect rainbowEffect;

  SpectrumWaveEffect spectrumWaveEffect;

  TwinkleStarsEffect twinkleStarsEffect;

  GradientFlowEffect gradientFlowEffect;

  LavaLampEffect lavaLampEffect;

  PlasmaEffect plasmaEffect;

  FireEffect fireEffect;

  NebulaEffect nebulaEffect;

  RowLedSyncEffect rowLedSyncEffect;

  GroupFillWaveEffect groupFillWaveEffect;

  CometEffect cometEffect;

  AlternatingStaticEffect alternatingStaticEffect;

  ColorCycleEffect colorCycleEffect;

  AuroraEffect auroraEffect;

  OceanWaveEffect oceanWaveEffect;
};