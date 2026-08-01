#include "SettingsStorage.h"

#include "Config.h"
#include "ModeManager.h"
#include "DecorativeEffectManager.h"
#include "PaletteManager.h"
#include "StaticColorEffect.h"

namespace
{
  SettingsStorage storageInstance;
}

SettingsStorage& settingsStorage()
{
  return storageInstance;
}

void SettingsStorage::begin()
{
  resetToDefaults();

  ready =
    preferences.begin(
      "argb-state",
      false
    );

  if (!ready)
  {
    Serial.println(
      "Persistent settings: unavailable"
    );

    return;
  }

  load();
}

void SettingsStorage::update()
{
  if (
    !ready ||
    !dirty
  )
  {
    return;
  }

  if (
    millis() - lastChangeAt <
      SAVE_DELAY_MS
  )
  {
    return;
  }

  save();
}

bool SettingsStorage::isReady() const
{
  return ready;
}

void SettingsStorage::resetToDefaults()
{
  settings.version =
    SETTINGS_VERSION;

  // SystemMode::Decorative
  settings.mode = 0;

  settings.brightness =
    DEFAULT_BRIGHTNESS;

  settings.decorativeSpeed = 35;

  // DecorativeEffectType::StaticColor
  settings.decorativeEffect = 0;

  for (
    uint8_t index = 0;
    index <
      PersistentSettings::
        DECORATIVE_PATTERN_SLOTS;
    index++
  )
  {
    settings.decorativePatterns[
      index
    ] = 0;
  }

  // DecorativePaletteType::Ocean
  settings.palette = 1;

  settings.paletteReversed = 0;

  // White
  settings.staticColorIndex = 0;

  dirty = false;

  lastChangeAt = 0;
}

void SettingsStorage::load()
{
  const size_t storedLength =
    preferences.getBytesLength(
      "settings"
    );

  if (
    storedLength !=
      sizeof(PersistentSettings)
  )
  {
    Serial.println(
      "Persistent settings: defaults"
    );

    save();

    return;
  }

  PersistentSettings loaded;

  const size_t bytesRead =
    preferences.getBytes(
      "settings",
      &loaded,
      sizeof(loaded)
    );

  if (
    bytesRead != sizeof(loaded)
  )
  {
    Serial.println(
      "Persistent settings: read failed"
    );

    resetToDefaults();

    save();

    return;
  }

  if (!migrateIfNeeded(loaded))
  {
    Serial.println(
      "Persistent settings: unsupported version"
    );

    resetToDefaults();

    save();

    return;
  }

  settings = loaded;

  if (!valuesAreValid())
  {
    Serial.println(
      "Persistent settings: invalid, using defaults"
    );

    resetToDefaults();

    save();

    return;
  }

  dirty = false;

  Serial.println(
    "Persistent settings: restored"
  );
}

void SettingsStorage::save()
{
  if (!ready)
  {
    return;
  }

  settings.version =
    SETTINGS_VERSION;

  const size_t bytesWritten =
    preferences.putBytes(
      "settings",
      &settings,
      sizeof(settings)
    );

  if (
    bytesWritten ==
      sizeof(settings)
  )
  {
    dirty = false;

    Serial.println(
      "Persistent settings: saved"
    );
  }
  else
  {
    Serial.println(
      "Persistent settings: save failed"
    );
  }
}

void SettingsStorage::markDirty()
{
  if (!ready)
  {
    return;
  }

  dirty = true;

  lastChangeAt = millis();
}

bool SettingsStorage::migrateIfNeeded(
  PersistentSettings& loaded
)
{
  if (
    loaded.version ==
      SETTINGS_VERSION
  )
  {
    return true;
  }

  /*
    Future migrations go here.

    Example:

    if (loaded.version == 1)
    {
      // Convert Version 1 values to Version 2.
      loaded.version = 2;
    }
  */

  return false;
}

bool SettingsStorage::valuesAreValid() const
{
  if (
    settings.version !=
      SETTINGS_VERSION
  )
  {
    return false;
  }

  if (
    settings.mode >
      static_cast<uint8_t>(
        SystemMode::Off
      )
  )
  {
    return false;
  }

  if (
    settings.brightness < 10 ||
    settings.brightness > 255
  )
  {
    return false;
  }

  if (
    settings.decorativeSpeed < 1 ||
    settings.decorativeSpeed > 100
  )
  {
    return false;
  }

  if (
    settings.decorativeEffect >
      static_cast<uint8_t>(
        DecorativeEffectType::OceanWave
      )
  )
  {
    return false;
  }

  for (
    uint8_t index = 0;
    index <
      PersistentSettings::
        DECORATIVE_PATTERN_SLOTS;
    index++
  )
  {
    if (
      settings.decorativePatterns[
        index
      ] > 31
    )
    {
      return false;
    }
  }

  if (
    settings.palette >=
      PaletteManager::getPaletteCount()
  )
  {
    return false;
  }

  if (settings.paletteReversed > 1)
  {
    return false;
  }

  if (
    settings.staticColorIndex >=
      StaticColorEffect::getColorCount()
  )
  {
    return false;
  }

  return true;
}

uint8_t SettingsStorage::getMode() const
{
  return settings.mode;
}

uint8_t SettingsStorage::
getBrightness() const
{
  return settings.brightness;
}

uint8_t SettingsStorage::
getDecorativeSpeed() const
{
  return settings.decorativeSpeed;
}

uint8_t SettingsStorage::
getDecorativeEffect() const
{
  return settings.decorativeEffect;
}

uint8_t SettingsStorage::
getDecorativePattern(
  uint8_t effectIndex
) const
{
  if (
    effectIndex >=
      PersistentSettings::
        DECORATIVE_PATTERN_SLOTS
  )
  {
    return 0;
  }

  return settings.decorativePatterns[
    effectIndex
  ];
}

uint8_t SettingsStorage::getPalette() const
{
  return settings.palette;
}

bool SettingsStorage::
getPaletteReversed() const
{
  return settings.paletteReversed != 0;
}

uint8_t SettingsStorage::
getStaticColorIndex() const
{
  return settings.staticColorIndex;
}

void SettingsStorage::setMode(
  uint8_t value
)
{
  if (
    value >
      static_cast<uint8_t>(
        SystemMode::Off
      ) ||
    settings.mode == value
  )
  {
    return;
  }

  settings.mode = value;

  markDirty();
}

void SettingsStorage::setBrightness(
  uint8_t value
)
{
  value = constrain(
    value,
    10,
    255
  );

  if (settings.brightness == value)
  {
    return;
  }

  settings.brightness = value;

  markDirty();
}

void SettingsStorage::setDecorativeSpeed(
  uint8_t value
)
{
  value = constrain(
    value,
    1,
    100
  );

  if (
    settings.decorativeSpeed ==
      value
  )
  {
    return;
  }

  settings.decorativeSpeed = value;

  markDirty();
}

void SettingsStorage::setDecorativeEffect(
  uint8_t value
)
{
  if (
    value >
      static_cast<uint8_t>(
        DecorativeEffectType::OceanWave
      ) ||
    settings.decorativeEffect ==
      value
  )
  {
    return;
  }

  settings.decorativeEffect = value;

  markDirty();
}

void SettingsStorage::
setDecorativePattern(
  uint8_t effectIndex,
  uint8_t patternIndex
)
{
  if (
    effectIndex >=
      PersistentSettings::
        DECORATIVE_PATTERN_SLOTS ||
    patternIndex > 31 ||
    settings.decorativePatterns[
      effectIndex
    ] == patternIndex
  )
  {
    return;
  }

  settings.decorativePatterns[
    effectIndex
  ] = patternIndex;

  markDirty();
}

void SettingsStorage::setPalette(
  uint8_t value
)
{
  if (
    value >=
      PaletteManager::getPaletteCount() ||
    settings.palette == value
  )
  {
    return;
  }

  settings.palette = value;

  markDirty();
}

void SettingsStorage::setPaletteReversed(
  bool value
)
{
  const uint8_t storedValue =
    value ? 1 : 0;

  if (
    settings.paletteReversed ==
      storedValue
  )
  {
    return;
  }

  settings.paletteReversed =
    storedValue;

  markDirty();
}

void SettingsStorage::setStaticColorIndex(
  uint8_t value
)
{
  if (
    value >=
      StaticColorEffect::getColorCount() ||
    settings.staticColorIndex ==
      value
  )
  {
    return;
  }

  settings.staticColorIndex = value;

  markDirty();
}
