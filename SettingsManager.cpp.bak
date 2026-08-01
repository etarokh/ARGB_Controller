#include "SettingsManager.h"

void SettingsManager::begin()
{
  selectedItem =
    SettingsItem::Brightness;
}

void SettingsManager::nextItem()
{
  switch (selectedItem)
  {
    case SettingsItem::Brightness:
      selectedItem =
        SettingsItem::EffectSpeed;
      break;

    case SettingsItem::EffectSpeed:
      selectedItem =
        SettingsItem::Brightness;
      break;
  }

  printStatus();
}

SettingsItem
SettingsManager::getSelectedItem() const
{
  return selectedItem;
}

const char*
SettingsManager::getSelectedItemName() const
{
  switch (selectedItem)
  {
    case SettingsItem::Brightness:
      return "Brightness";

    case SettingsItem::EffectSpeed:
      return "Effect Speed";
  }

  return "Unknown";
}

void SettingsManager::printStatus() const
{
  Serial.println();
  Serial.println("SETTINGS");
  Serial.println();

  if (
    selectedItem ==
    SettingsItem::Brightness
  )
  {
    Serial.println("> Brightness");
    Serial.println("  Effect Speed");
  }
  else
  {
    Serial.println("  Brightness");
    Serial.println("> Effect Speed");
  }
}
