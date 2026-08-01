#pragma once

#include <Arduino.h>

enum class SettingsItem
{
  Brightness,
  EffectSpeed
};

class SettingsManager
{
public:
  void begin();

  void nextItem();

  SettingsItem getSelectedItem() const;

  const char* getSelectedItemName() const;

  void printStatus() const;

private:
  SettingsItem selectedItem =
    SettingsItem::Brightness;
};
