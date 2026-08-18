#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include <Preferences.h>

#include "GaugeRenderer.h"

class LedManager;

enum class MonitorPage : uint8_t
{
  SystemOverview = 0,
  Network,
  Services
};

class MonitorManager
{
public:
  static constexpr uint8_t THEME_COUNT = 6;

  void begin(
    LedManager* ledManager
  );

  void update();

  void setEnabled(
    bool enabled
  );

  bool isEnabled() const;

  void forceRender();

  void suppressNextStartupSweep();

  void resetToFirstPage();

  void nextMonitorPage();

  MonitorPage getMonitorPage() const;

  const char* getMonitorPageName() const;

  void setCpuUsagePercent(
    uint8_t percent
  );

  uint8_t getCpuUsagePercent() const;

  void setStorageUsedPercent(
    uint8_t percent
  );

  uint8_t getStorageUsedPercent() const;

  void setStorageCapacity(
    float freeTb,
    float totalTb
  );

  float getStorageFreeTb() const;

  float getStorageTotalTb() const;

  void setTheme(
    uint8_t theme
  );

  void nextTheme();

  uint8_t getTheme() const;

  const char* getThemeName() const;

  void setStorageGaugeColorIndex(
    uint8_t colorIndex
  );

  uint8_t getStorageGaugeColorIndex() const;

  void setBaseGaugeColorIndex(
    uint8_t colorIndex
  );

  uint8_t
  getBaseGaugeColorIndex() const;

  void setCpuGaugeColorIndex(
    uint8_t colorIndex
  );

  uint8_t getCpuGaugeColorIndex() const;

private:
  LedManager* leds = nullptr;

  GaugeRenderer gaugeRenderer;

  Preferences preferences;

  bool preferencesReady = false;

  bool enabled = false;

  bool needsRender = true;

  MonitorPage currentPage =
    MonitorPage::SystemOverview;

  uint8_t cpuUsagePercent = 50;

  uint8_t displayedCpuUsagePercent = 50;

  uint8_t storageUsedPercent = 25;

  uint8_t displayedStorageUsedPercent = 25;

  unsigned long lastGaugeAnimationAt = 0;

  static constexpr unsigned long
    GAUGE_ANIMATION_INTERVAL_MS = 18UL;

  static constexpr unsigned long
    STARTUP_SWEEP_OUT_INTERVAL_MS = 18UL;

  static constexpr unsigned long
    STARTUP_SWEEP_RETURN_INTERVAL_MS = 38UL;

  static constexpr uint8_t
    STARTUP_SWEEP_OUT_STEP = 4;

  static constexpr uint8_t
    STARTUP_SWEEP_RETURN_STEP = 1;

  bool startupSweepActive = false;

  bool startupSweepReturning = false;

  bool suppressNextSweep = false;

  unsigned long lastStartupSweepAt = 0;

  float storageFreeTb = 6.0f;

  float storageTotalTb = 8.0f;

  uint8_t themeIndex = 0;

  uint8_t storageGaugeColorIndex = 0;

  uint8_t baseGaugeColorIndex = 0;

  uint8_t cpuGaugeColorIndex = 0;

  void render();

  void beginStartupSweep();

  void updateStartupSweep();

  void renderSystemOverview();

  void renderPlaceholder(
    const CRGB& color
  );

  CRGB getBackgroundColor() const;

  CRGB getStorageColor() const;

  CRGB getCpuColor() const;

  CRGB getInactiveGroupColor() const;
};
