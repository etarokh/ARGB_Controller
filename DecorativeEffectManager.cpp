#include "DecorativeEffectManager.h"
#include "Config.h"
#include "SettingsStorage.h"

void DecorativeEffectManager::begin(
  LedManager* ledManager
)
{
  leds = ledManager;
  enabled = false;

  decorativeSpeed = 50;

  paletteManager.begin();

  staticColorEffect.begin(
    ledManager
  );

  breathingEffect.begin(
    ledManager
  );

  rainbowEffect.begin(
    ledManager
  );

  spectrumWaveEffect.begin(
    ledManager
  );

  twinkleStarsEffect.begin(
    ledManager
  );

  gradientFlowEffect.begin(
    ledManager
  );

  lavaLampEffect.begin(
    ledManager
  );

  plasmaEffect.begin(
    ledManager
  );

  fireEffect.begin(
    ledManager
  );

  nebulaEffect.begin(
    ledManager
  );

  rowLedSyncEffect.begin(
    ledManager
  );

  rowLedSyncEffect.setPattern(
    settingsStorage().
      getDecorativePattern(
        static_cast<uint8_t>(
          DecorativeEffectType::RowLedSync
        )
      )
  );

  groupFillWaveEffect.begin(
    ledManager
  );

  cometEffect.begin(
    ledManager
  );

  alternatingStaticEffect.begin(
    ledManager
  );

  colorCycleEffect.begin(
    ledManager
  );

  auroraEffect.begin(
    ledManager
  );

  oceanWaveEffect.begin(
    ledManager
  );

  colorCycleEffect.setPaletteManager(
    &paletteManager
  );

  groupFillWaveEffect.setPaletteManager(
    &paletteManager
  );

  auroraEffect.setPaletteManager(
    &paletteManager
  );

  oceanWaveEffect.setPaletteManager(
    &paletteManager
  );

  applySpeedToEffects();

  currentEffectType =
    DecorativeEffectType::StaticColor;

  selectCurrentEffect();
}

void DecorativeEffectManager::update()
{
  if (!enabled)
  {
    return;
  }

  if (currentEffect == nullptr)
  {
    return;
  }

  currentEffect->update();
}

void DecorativeEffectManager::setEnabled(
  bool newEnabled
)
{
  if (enabled == newEnabled)
  {
    return;
  }

  enabled = newEnabled;

  if (enabled)
  {
    if (currentEffect != nullptr)
    {
      currentEffect->reset();
    }
  }
  else
  {
    clear();
  }
}

bool DecorativeEffectManager::isEnabled() const
{
  return enabled;
}

void DecorativeEffectManager::setEffect(
  DecorativeEffectType effect
)
{
  currentEffectType = effect;

  settingsStorage().
    setDecorativeEffect(
      static_cast<uint8_t>(
        currentEffectType
      )
    );

  selectCurrentEffect();

  if (currentEffect != nullptr)
  {
    currentEffect->reset();
  }

  Serial.print(
    "Decorative effect: "
  );

  Serial.println(
    getEffectName()
  );
}

DecorativeEffectType
DecorativeEffectManager::getEffect() const
{
  return currentEffectType;
}

void DecorativeEffectManager::nextEffect()
{
  uint8_t currentIndex =
    static_cast<uint8_t>(
      currentEffectType
    );

  currentIndex++;

  if (currentIndex >= EFFECT_COUNT)
  {
    currentIndex = 0;
  }

  setEffect(
    static_cast<DecorativeEffectType>(
      currentIndex
    )
  );
}


void DecorativeEffectManager::nextStaticColor()
{
  staticColorEffect.nextColor();

  Serial.print(
    "Static color: "
  );

  Serial.println(
    staticColorEffect.getColorName()
  );
}

void DecorativeEffectManager::setStaticColorIndex(
  uint8_t index
)
{
  staticColorEffect.setColorIndex(
    index
  );

  settingsStorage().
    setStaticColorIndex(
      staticColorEffect.getColorIndex()
    );

  Serial.print(
    "Static color: "
  );

  Serial.println(
    staticColorEffect.getColorName()
  );
}

uint8_t
DecorativeEffectManager::
getStaticColorIndex() const
{
  return staticColorEffect.getColorIndex();
}

bool DecorativeEffectManager::hasPattern() const
{
  return getPatternCount() > 0;
}

uint8_t DecorativeEffectManager::getPattern() const
{
  switch (currentEffectType)
  {
    case DecorativeEffectType::SpectrumWave:
      return spectrumWaveEffect.getPattern();

    case DecorativeEffectType::TwinkleStars:
      return twinkleStarsEffect.getPattern();

    case DecorativeEffectType::GradientFlow:
      return gradientFlowEffect.getPattern();

    case DecorativeEffectType::LavaLamp:
      return lavaLampEffect.getPattern();

    case DecorativeEffectType::Plasma:
      return plasmaEffect.getPattern();

    case DecorativeEffectType::Fire:
      return fireEffect.getPattern();

    case DecorativeEffectType::Nebula:
      return nebulaEffect.getPattern();

    case DecorativeEffectType::RowLedSync:
      return rowLedSyncEffect.getPattern();

    case DecorativeEffectType::GroupFillWave:
      return groupFillWaveEffect.getPattern();

    case DecorativeEffectType::AlternatingStatic:
      return alternatingStaticEffect.getPattern();

    default:
      return 0;
  }
}

uint8_t DecorativeEffectManager::getPatternCount() const
{
  switch (currentEffectType)
  {
    case DecorativeEffectType::SpectrumWave:
    case DecorativeEffectType::TwinkleStars:
    case DecorativeEffectType::GradientFlow:
    case DecorativeEffectType::LavaLamp:
    case DecorativeEffectType::Plasma:
    case DecorativeEffectType::Fire:
    case DecorativeEffectType::Nebula:
    case DecorativeEffectType::GroupFillWave:
      return 4;

    case DecorativeEffectType::RowLedSync:
      return 10;

    case DecorativeEffectType::AlternatingStatic:
      return 12;

    default:
      return 0;
  }
}

void DecorativeEffectManager::setPattern(
  uint8_t pattern
)
{
  const uint8_t patternCount =
    getPatternCount();

  if (patternCount == 0)
  {
    return;
  }

  if (pattern >= patternCount)
  {
    pattern = 0;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::RowLedSync
  )
  {
    rowLedSyncEffect.setPattern(
      pattern
    );

    settingsStorage().
      setDecorativePattern(
        static_cast<uint8_t>(
          DecorativeEffectType::RowLedSync
        ),
        rowLedSyncEffect.getPattern()
      );

    return;
  }

  for (
    uint8_t step = 0;
    step < patternCount;
    step++
  )
  {
    if (getPattern() == pattern)
    {
      return;
    }

    switch (currentEffectType)
    {
      case DecorativeEffectType::SpectrumWave:
        spectrumWaveEffect.nextPattern();
        break;

      case DecorativeEffectType::TwinkleStars:
        twinkleStarsEffect.nextPattern();
        break;

      case DecorativeEffectType::GradientFlow:
        gradientFlowEffect.nextPattern();
        break;

      case DecorativeEffectType::LavaLamp:
        lavaLampEffect.nextPattern();
        break;

      case DecorativeEffectType::Plasma:
        plasmaEffect.nextPattern();
        break;

      case DecorativeEffectType::Fire:
        fireEffect.nextPattern();
        break;

      case DecorativeEffectType::Nebula:
        nebulaEffect.nextPattern();
        break;

      case DecorativeEffectType::GroupFillWave:
        groupFillWaveEffect.nextPattern();
        break;

      case DecorativeEffectType::AlternatingStatic:
        alternatingStaticEffect.nextPattern();
        break;

      default:
        return;
    }
  }
}

void DecorativeEffectManager::nextPattern()
{
  if (
    currentEffectType ==
      DecorativeEffectType::RowLedSync
  )
  {
    rowLedSyncEffect.nextPattern();

    settingsStorage().
      setDecorativePattern(
        static_cast<uint8_t>(
          DecorativeEffectType::RowLedSync
        ),
        rowLedSyncEffect.getPattern()
      );

    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::SpectrumWave
  )
  {
    spectrumWaveEffect.nextPattern();
    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::TwinkleStars
  )
  {
    twinkleStarsEffect.nextPattern();
    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::GradientFlow
  )
  {
    gradientFlowEffect.nextPattern();
    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::LavaLamp
  )
  {
    lavaLampEffect.nextPattern();
    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::Plasma
  )
  {
    plasmaEffect.nextPattern();
    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::Fire
  )
  {
    fireEffect.nextPattern();
    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::Nebula
  )
  {
    nebulaEffect.nextPattern();
    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::GroupFillWave
  )
  {
    groupFillWaveEffect.nextPattern();
    return;
  }

  if (
    currentEffectType ==
      DecorativeEffectType::AlternatingStatic
  )
  {
    alternatingStaticEffect.nextPattern();
  }
}

const char*
DecorativeEffectManager::getPatternName() const
{
  if (
    currentEffectType ==
      DecorativeEffectType::RowLedSync
  )
  {
    return rowLedSyncEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::SpectrumWave
  )
  {
    return spectrumWaveEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::TwinkleStars
  )
  {
    return twinkleStarsEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::GradientFlow
  )
  {
    return gradientFlowEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::LavaLamp
  )
  {
    return lavaLampEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::Plasma
  )
  {
    return plasmaEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::Fire
  )
  {
    return fireEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::Nebula
  )
  {
    return nebulaEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::GroupFillWave
  )
  {
    return groupFillWaveEffect.getPatternName();
  }

  if (
    currentEffectType ==
      DecorativeEffectType::AlternatingStatic
  )
  {
    return alternatingStaticEffect.getPatternName();
  }

  return "N/A";
}

const char*
DecorativeEffectManager::getStaticColorName() const
{
  return staticColorEffect.getColorName();
}


void DecorativeEffectManager::previousEffect()
{
  uint8_t currentIndex =
    static_cast<uint8_t>(
      currentEffectType
    );

  if (currentIndex == 0)
  {
    currentIndex =
      EFFECT_COUNT - 1;
  }
  else
  {
    currentIndex--;
  }

  setEffect(
    static_cast<DecorativeEffectType>(
      currentIndex
    )
  );
}

const char*
DecorativeEffectManager::getEffectName() const
{
  switch (currentEffectType)
  {
    case DecorativeEffectType::StaticColor:
      return "Static Color";

    case DecorativeEffectType::Breathing:
      return "Breathing";

    case DecorativeEffectType::Rainbow:
      return "Rainbow";

    case DecorativeEffectType::SpectrumWave:
      return "Spectrum Wave";

    case DecorativeEffectType::TwinkleStars:
      return "Twinkle Stars";

    case DecorativeEffectType::GradientFlow:
      return "Gradient Flow";

    case DecorativeEffectType::LavaLamp:
      return "Lava Lamp";

    case DecorativeEffectType::Plasma:
      return "Plasma";

    case DecorativeEffectType::Fire:
      return "Fire";

    case DecorativeEffectType::Nebula:
      return "Nebula";

    case DecorativeEffectType::RowLedSync:
      return "Row LED Sync";

    case DecorativeEffectType::GroupFillWave:
      return "Group Fill Wave";

    case DecorativeEffectType::Comet:
      return "Comet";

    case DecorativeEffectType::AlternatingStatic:
      return "Alternating Static";

    case DecorativeEffectType::ColorCycle:
      return "Color Cycle";

    case DecorativeEffectType::Aurora:
      return "Aurora";

    case DecorativeEffectType::OceanWave:
      return "Ocean Wave";

    default:
      return "Unknown";
  }
}

void DecorativeEffectManager::setColor(
  const CRGB& color
)
{
  staticColorEffect.setColor(
    color
  );

  breathingEffect.setColor(
    color
  );

  rainbowEffect.setColor(
    color
  );

  spectrumWaveEffect.setColor(
    color
  );

  twinkleStarsEffect.setColor(
    color
  );

  gradientFlowEffect.setColor(
    color
  );

  lavaLampEffect.setColor(
    color
  );

  plasmaEffect.setColor(
    color
  );

  fireEffect.setColor(
    color
  );

  nebulaEffect.setColor(
    color
  );

  rowLedSyncEffect.setColor(
    color
  );

  groupFillWaveEffect.setColor(
    color
  );

  cometEffect.setColor(
    color
  );

  alternatingStaticEffect.setColor(
    color
  );

  colorCycleEffect.setColor(
    color
  );

  auroraEffect.setColor(
    color
  );

  oceanWaveEffect.setColor(
    color
  );

  if (currentEffect != nullptr)
  {
    currentEffect->reset();
  }
}

void DecorativeEffectManager::setPalette(
  DecorativePaletteType paletteType
)
{
  paletteManager.setPalette(
    paletteType
  );

  settingsStorage().setPalette(
    static_cast<uint8_t>(
      paletteManager.getPaletteType()
    )
  );

  if (currentEffect != nullptr)
  {
    currentEffect->reset();
  }

  Serial.print(
    "Decorative palette: "
  );

  Serial.println(
    getPaletteName()
  );
}

DecorativePaletteType
DecorativeEffectManager::getPaletteType() const
{
  return paletteManager.getPaletteType();
}

void DecorativeEffectManager::nextPalette()
{
  paletteManager.nextPalette();

  settingsStorage().setPalette(
    static_cast<uint8_t>(
      paletteManager.getPaletteType()
    )
  );

  if (currentEffect != nullptr)
  {
    currentEffect->reset();
  }

  Serial.print(
    "Decorative palette: "
  );

  Serial.println(
    getPaletteName()
  );
}

void DecorativeEffectManager::previousPalette()
{
  paletteManager.previousPalette();

  settingsStorage().setPalette(
    static_cast<uint8_t>(
      paletteManager.getPaletteType()
    )
  );

  if (currentEffect != nullptr)
  {
    currentEffect->reset();
  }

  Serial.print(
    "Decorative palette: "
  );

  Serial.println(
    getPaletteName()
  );
}

void DecorativeEffectManager::setPaletteReversed(
  bool reversed
)
{
  paletteManager.setReversed(
    reversed
  );

  settingsStorage().
    setPaletteReversed(
      paletteManager.isReversed()
    );

  if (currentEffect != nullptr)
  {
    currentEffect->reset();
  }
}

void DecorativeEffectManager::
togglePaletteReversed()
{
  paletteManager.toggleReversed();

  settingsStorage().
    setPaletteReversed(
      paletteManager.isReversed()
    );

  if (currentEffect != nullptr)
  {
    currentEffect->reset();
  }
}

bool DecorativeEffectManager::
isPaletteReversed() const
{
  return paletteManager.isReversed();
}

const char*
DecorativeEffectManager::getPaletteName() const
{
  return paletteManager.getPaletteName();
}

void DecorativeEffectManager::setSpeed(
  uint8_t speedPercent
)
{
  if (speedPercent < 1)
  {
    speedPercent = 1;
  }

  if (speedPercent > 100)
  {
    speedPercent = 100;
  }

  decorativeSpeed = speedPercent;

  settingsStorage().
    setDecorativeSpeed(
      decorativeSpeed
    );

  applySpeedToEffects();

  Serial.print(
    "Decorative speed: "
  );

  Serial.print(
    decorativeSpeed
  );

  Serial.println("%");
}

uint8_t
DecorativeEffectManager::getSpeed() const
{
  return decorativeSpeed;
}

void DecorativeEffectManager::increaseSpeed(
  uint8_t amount
)
{
  uint16_t newSpeed =
    decorativeSpeed + amount;

  if (newSpeed > 100)
  {
    newSpeed = 100;
  }

  setSpeed(
    static_cast<uint8_t>(
      newSpeed
    )
  );
}

void DecorativeEffectManager::decreaseSpeed(
  uint8_t amount
)
{
  int16_t newSpeed =
    decorativeSpeed - amount;

  if (newSpeed < 1)
  {
    newSpeed = 1;
  }

  setSpeed(
    static_cast<uint8_t>(
      newSpeed
    )
  );
}

void DecorativeEffectManager::
applySpeedToEffects()
{
  rainbowEffect.setSpeed(
    decorativeSpeed
  );

  spectrumWaveEffect.setSpeed(
    decorativeSpeed
  );

  twinkleStarsEffect.setSpeed(
    decorativeSpeed
  );

  gradientFlowEffect.setSpeed(
    decorativeSpeed
  );

  lavaLampEffect.setSpeed(
    decorativeSpeed
  );

  plasmaEffect.setSpeed(
    decorativeSpeed
  );

  fireEffect.setSpeed(
    decorativeSpeed
  );

  nebulaEffect.setSpeed(
    decorativeSpeed
  );

  rowLedSyncEffect.setSpeed(
    decorativeSpeed
  );

  groupFillWaveEffect.setSpeed(
    decorativeSpeed
  );

  cometEffect.setSpeed(
    decorativeSpeed
  );

  alternatingStaticEffect.setSpeed(
    decorativeSpeed
  );

  breathingEffect.setSpeed(
    decorativeSpeed
  );

  colorCycleEffect.setSpeed(
    decorativeSpeed
  );

  auroraEffect.setSpeed(
    decorativeSpeed
  );

  oceanWaveEffect.setSpeed(
    decorativeSpeed
  );
}

void DecorativeEffectManager::clear()
{
  if (leds == nullptr)
  {
    return;
  }

  leds->setEffectBrightness(
    255
  );

  leds->clear();
}

void DecorativeEffectManager::
selectCurrentEffect()
{
  switch (currentEffectType)
  {
    case DecorativeEffectType::StaticColor:
      currentEffect =
        &staticColorEffect;
      break;

    case DecorativeEffectType::Breathing:
      currentEffect =
        &breathingEffect;
      break;

    case DecorativeEffectType::Rainbow:
      currentEffect =
        &rainbowEffect;
      break;

    case DecorativeEffectType::SpectrumWave:
      currentEffect =
        &spectrumWaveEffect;
      break;

    case DecorativeEffectType::TwinkleStars:
      currentEffect =
        &twinkleStarsEffect;
      break;

    case DecorativeEffectType::GradientFlow:
      currentEffect =
        &gradientFlowEffect;
      break;

    case DecorativeEffectType::LavaLamp:
      currentEffect =
        &lavaLampEffect;
      break;

    case DecorativeEffectType::Plasma:
      currentEffect =
        &plasmaEffect;
      break;

    case DecorativeEffectType::Fire:
      currentEffect =
        &fireEffect;
      break;

    case DecorativeEffectType::Nebula:
      currentEffect =
        &nebulaEffect;
      break;

    case DecorativeEffectType::RowLedSync:
      currentEffect =
        &rowLedSyncEffect;
      break;

    case DecorativeEffectType::GroupFillWave:
      currentEffect =
        &groupFillWaveEffect;
      break;

    case DecorativeEffectType::Comet:
      currentEffect =
        &cometEffect;
      break;

    case DecorativeEffectType::AlternatingStatic:
      currentEffect =
        &alternatingStaticEffect;
      break;

    case DecorativeEffectType::ColorCycle:
      currentEffect =
        &colorCycleEffect;
      break;

    case DecorativeEffectType::Aurora:
      currentEffect =
        &auroraEffect;
      break;

    case DecorativeEffectType::OceanWave:
      currentEffect =
        &oceanWaveEffect;
      break;

    default:
      currentEffectType =
        DecorativeEffectType::StaticColor;

      currentEffect =
        &staticColorEffect;
      break;
  }
}