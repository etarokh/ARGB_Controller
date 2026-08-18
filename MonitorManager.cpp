#include "MonitorManager.h"

#include "LedManager.h"
#include "Config.h"

void MonitorManager::begin(
  LedManager* ledManager
)
{
  leds = ledManager;

  enabled = false;

  currentPage =
    MonitorPage::SystemOverview;

  cpuUsagePercent = 50;

  displayedCpuUsagePercent =
    cpuUsagePercent;

  storageUsedPercent = 25;

  displayedStorageUsedPercent =
    storageUsedPercent;

  lastGaugeAnimationAt = 0;

  startupSweepActive = false;

  startupSweepReturning = false;

  suppressNextSweep = false;

  lastStartupSweepAt = 0;

  storageFreeTb = 6.0f;

  storageTotalTb = 8.0f;

  preferencesReady =
    preferences.begin(
      "monitor-mode",
      false
    );

  if (preferencesReady)
  {
    themeIndex =
      preferences.getUChar(
        "theme",
        0
      );

    if (themeIndex >= THEME_COUNT)
    {
      themeIndex = 0;

      preferences.putUChar(
        "theme",
        themeIndex
      );
    }

    storageGaugeColorIndex =
      preferences.getUChar(
        "storageColor",
        themeIndex
      );

    if (
      storageGaugeColorIndex >=
        THEME_COUNT
    )
    {
      storageGaugeColorIndex =
        themeIndex;

      preferences.putUChar(
        "storageColor",
        storageGaugeColorIndex
      );
    }

    baseGaugeColorIndex =
      preferences.getUChar(
        "baseColor",
        themeIndex
      );

    if (
      baseGaugeColorIndex >=
        THEME_COUNT
    )
    {
      baseGaugeColorIndex =
        themeIndex;

      preferences.putUChar(
        "baseColor",
        baseGaugeColorIndex
      );
    }

    cpuGaugeColorIndex =
      preferences.getUChar(
        "cpuColor",
        themeIndex
      );

    if (
      cpuGaugeColorIndex >=
        THEME_COUNT
    )
    {
      cpuGaugeColorIndex =
        themeIndex;

      preferences.putUChar(
        "cpuColor",
        cpuGaugeColorIndex
      );
    }

    Serial.print(
      "MONITOR THEME RESTORED: "
    );

    Serial.println(
      getThemeName()
    );
  }
  else
  {
    themeIndex = 0;

    storageGaugeColorIndex = 0;

    baseGaugeColorIndex = 0;

    cpuGaugeColorIndex = 0;

    Serial.println(
      "MONITOR THEME: NVS unavailable"
    );
  }

  needsRender = true;
}

void MonitorManager::update()
{
  if (
    !enabled ||
    leds == nullptr
  )
  {
    return;
  }

  if (startupSweepActive)
  {
    updateStartupSweep();

    if (needsRender)
    {
      render();

      needsRender = false;
    }

    return;
  }

  const unsigned long now =
    millis();

  if (
    now - lastGaugeAnimationAt >=
      GAUGE_ANIMATION_INTERVAL_MS
  )
  {
    lastGaugeAnimationAt = now;

    auto moveToward = [](
      uint8_t currentValue,
      uint8_t targetValue
    ) -> uint8_t
    {
      if (currentValue == targetValue)
      {
        return currentValue;
      }

      const uint8_t difference =
        currentValue > targetValue
          ? currentValue - targetValue
          : targetValue - currentValue;

      uint8_t step = 1;

      if (difference >= 40)
      {
        step = 10;
      }
      else if (difference >= 20)
      {
        step = 6;
      }
      else if (difference >= 8)
      {
        step = 3;
      }
      else if (difference >= 3)
      {
        step = 2;
      }

      if (currentValue < targetValue)
      {
        if (
          targetValue - currentValue <=
            step
        )
        {
          return targetValue;
        }

        return currentValue + step;
      }

      if (
        currentValue - targetValue <=
          step
      )
      {
        return targetValue;
      }

      return currentValue - step;
    };

    const uint8_t newCpuValue =
      moveToward(
        displayedCpuUsagePercent,
        cpuUsagePercent
      );

    const uint8_t newStorageValue =
      moveToward(
        displayedStorageUsedPercent,
        storageUsedPercent
      );

    if (
      newCpuValue !=
        displayedCpuUsagePercent
    )
    {
      displayedCpuUsagePercent =
        newCpuValue;

      needsRender = true;
    }

    if (
      newStorageValue !=
        displayedStorageUsedPercent
    )
    {
      displayedStorageUsedPercent =
        newStorageValue;

      needsRender = true;
    }
  }

  if (!needsRender)
  {
    return;
  }

  render();

  needsRender = false;
}

void MonitorManager::setEnabled(
  bool newEnabled
)
{
  if (enabled == newEnabled)
  {
    return;
  }

  enabled = newEnabled;

  if (!enabled)
  {
    startupSweepActive = false;

    startupSweepReturning = false;

    return;
  }

  if (leds == nullptr)
  {
    needsRender = true;

    return;
  }

  if (suppressNextSweep)
  {
    suppressNextSweep = false;

    startupSweepActive = false;

    startupSweepReturning = false;

    displayedCpuUsagePercent =
      cpuUsagePercent;

    displayedStorageUsedPercent =
      storageUsedPercent;

    forceRender();

    return;
  }

  beginStartupSweep();
}

bool MonitorManager::isEnabled() const
{
  return enabled;
}

void MonitorManager::forceRender()
{
  needsRender = true;

  if (
    enabled &&
    leds != nullptr
  )
  {
    render();

    needsRender = false;
  }
}

void MonitorManager::
suppressNextStartupSweep()
{
  suppressNextSweep = true;
}

void MonitorManager::resetToFirstPage()
{
  currentPage =
    MonitorPage::SystemOverview;

  needsRender = true;
}

void MonitorManager::nextMonitorPage()
{
  /*
   * Network and Service pages are reserved for
   * future implementation. Monitor currently stays
   * on the HDD + CPU system overview.
   */
  currentPage =
    MonitorPage::SystemOverview;

  needsRender = true;
}

MonitorPage
MonitorManager::getMonitorPage() const
{
  return currentPage;
}

const char*
MonitorManager::getMonitorPageName() const
{
  switch (currentPage)
  {
    case MonitorPage::SystemOverview:
      return "HDD + CPU";

    case MonitorPage::Network:
      return "NETWORK";

    case MonitorPage::Services:
      return "SERVICE";
  }

  return "UNKNOWN";
}

void MonitorManager::setCpuUsagePercent(
  uint8_t percent
)
{
  percent = constrain(
    percent,
    0,
    100
  );

  if (cpuUsagePercent == percent)
  {
    return;
  }

  cpuUsagePercent = percent;

  needsRender = true;
}

uint8_t
MonitorManager::getCpuUsagePercent() const
{
  return cpuUsagePercent;
}

void MonitorManager::setStorageUsedPercent(
  uint8_t percent
)
{
  percent = constrain(
    percent,
    0,
    100
  );

  if (
    storageUsedPercent ==
      percent
  )
  {
    return;
  }

  storageUsedPercent = percent;

  needsRender = true;
}

uint8_t
MonitorManager::getStorageUsedPercent() const
{
  return storageUsedPercent;
}

void MonitorManager::setStorageCapacity(
  float freeTb,
  float totalTb
)
{
  if (freeTb < 0.0f)
  {
    freeTb = 0.0f;
  }

  if (totalTb < 0.0f)
  {
    totalTb = 0.0f;
  }

  if (
    totalTb > 0.0f &&
    freeTb > totalTb
  )
  {
    freeTb = totalTb;
  }

  storageFreeTb = freeTb;

  storageTotalTb = totalTb;

  needsRender = true;
}

float
MonitorManager::getStorageFreeTb() const
{
  return storageFreeTb;
}

float
MonitorManager::getStorageTotalTb() const
{
  return storageTotalTb;
}

void MonitorManager::setTheme(
  uint8_t theme
)
{
  if (theme >= THEME_COUNT)
  {
    theme = 0;
  }

  if (themeIndex == theme)
  {
    return;
  }

  themeIndex = theme;

  storageGaugeColorIndex =
    themeIndex;

  baseGaugeColorIndex =
    themeIndex;

  cpuGaugeColorIndex =
    themeIndex;

  if (preferencesReady)
  {
    preferences.putUChar(
      "theme",
      themeIndex
    );

    preferences.putUChar(
      "storageColor",
      storageGaugeColorIndex
    );

    preferences.putUChar(
      "baseColor",
      baseGaugeColorIndex
    );

    preferences.putUChar(
      "cpuColor",
      cpuGaugeColorIndex
    );
  }

  needsRender = true;
}

void MonitorManager::nextTheme()
{
  themeIndex++;

  if (themeIndex >= THEME_COUNT)
  {
    themeIndex = 0;
  }

  storageGaugeColorIndex =
    themeIndex;

  baseGaugeColorIndex =
    themeIndex;

  cpuGaugeColorIndex =
    themeIndex;

  if (preferencesReady)
  {
    preferences.putUChar(
      "theme",
      themeIndex
    );

    preferences.putUChar(
      "storageColor",
      storageGaugeColorIndex
    );

    preferences.putUChar(
      "baseColor",
      baseGaugeColorIndex
    );

    preferences.putUChar(
      "cpuColor",
      cpuGaugeColorIndex
    );
  }

  needsRender = true;

  Serial.print(
    "MONITOR THEME: "
  );

  Serial.println(
    getThemeName()
  );
}

uint8_t MonitorManager::getTheme() const
{
  return themeIndex;
}

void MonitorManager::
setStorageGaugeColorIndex(
  uint8_t colorIndex
)
{
  if (colorIndex >= THEME_COUNT)
  {
    return;
  }

  if (
    storageGaugeColorIndex ==
      colorIndex
  )
  {
    return;
  }

  storageGaugeColorIndex =
    colorIndex;

  if (preferencesReady)
  {
    preferences.putUChar(
      "storageColor",
      storageGaugeColorIndex
    );
  }

  needsRender = true;
}

uint8_t MonitorManager::
getStorageGaugeColorIndex() const
{
  return storageGaugeColorIndex;
}

void MonitorManager::
setBaseGaugeColorIndex(
  uint8_t colorIndex
)
{
  if (colorIndex >= THEME_COUNT)
  {
    return;
  }

  if (
    baseGaugeColorIndex ==
      colorIndex
  )
  {
    return;
  }

  baseGaugeColorIndex =
    colorIndex;

  if (preferencesReady)
  {
    preferences.putUChar(
      "baseColor",
      baseGaugeColorIndex
    );
  }

  needsRender = true;
}

uint8_t MonitorManager::
getBaseGaugeColorIndex() const
{
  return baseGaugeColorIndex;
}

void MonitorManager::
setCpuGaugeColorIndex(
  uint8_t colorIndex
)
{
  if (colorIndex >= THEME_COUNT)
  {
    return;
  }

  if (
    cpuGaugeColorIndex ==
      colorIndex
  )
  {
    return;
  }

  cpuGaugeColorIndex =
    colorIndex;

  if (preferencesReady)
  {
    preferences.putUChar(
      "cpuColor",
      cpuGaugeColorIndex
    );
  }

  needsRender = true;
}

uint8_t MonitorManager::
getCpuGaugeColorIndex() const
{
  return cpuGaugeColorIndex;
}

const char*
MonitorManager::getThemeName() const
{
  switch (themeIndex)
  {
    case 0:
      return "WHITE / RED / BLUE";

    case 1:
      return "DARK / ORANGE / CYAN";

    case 2:
      return "VIOLET / GREEN / MAGENTA";

    case 3:
      return "ICE / BLUE / AQUA";

    case 4:
      return "AMBER / GOLD / RED";

    case 5:
      return "CYBER / LIME / PURPLE";

    default:
      return "UNKNOWN";
  }
}

void MonitorManager::beginStartupSweep()
{
  displayedCpuUsagePercent = 0;

  displayedStorageUsedPercent = 0;

  startupSweepActive = true;

  startupSweepReturning = false;

  lastStartupSweepAt = 0;

  needsRender = true;

  render();

  needsRender = false;

  Serial.println(
    "MONITOR GAUGE SWEEP: START"
  );
}

void MonitorManager::updateStartupSweep()
{
  const unsigned long now =
    millis();

  const unsigned long sweepInterval =
    startupSweepReturning
      ? STARTUP_SWEEP_RETURN_INTERVAL_MS
      : STARTUP_SWEEP_OUT_INTERVAL_MS;

  if (
    lastStartupSweepAt != 0 &&
    now - lastStartupSweepAt <
      sweepInterval
  )
  {
    return;
  }

  lastStartupSweepAt = now;

  if (!startupSweepReturning)
  {
    displayedCpuUsagePercent =
      min<uint8_t>(
        100,
        displayedCpuUsagePercent +
          STARTUP_SWEEP_OUT_STEP
      );

    displayedStorageUsedPercent =
      min<uint8_t>(
        100,
        displayedStorageUsedPercent +
          STARTUP_SWEEP_OUT_STEP
      );

    if (
      displayedCpuUsagePercent == 100 &&
      displayedStorageUsedPercent == 100
    )
    {
      startupSweepReturning = true;

      Serial.println(
        "MONITOR GAUGE SWEEP: PEAK"
      );
    }

    needsRender = true;

    return;
  }

  if (
    displayedCpuUsagePercent >
      cpuUsagePercent
  )
  {
    const uint8_t difference =
      displayedCpuUsagePercent -
      cpuUsagePercent;

    displayedCpuUsagePercent =
      difference <= STARTUP_SWEEP_RETURN_STEP
        ? cpuUsagePercent
        : displayedCpuUsagePercent -
            STARTUP_SWEEP_RETURN_STEP;
  }
  else if (
    displayedCpuUsagePercent <
      cpuUsagePercent
  )
  {
    const uint8_t difference =
      cpuUsagePercent -
      displayedCpuUsagePercent;

    displayedCpuUsagePercent =
      difference <= STARTUP_SWEEP_RETURN_STEP
        ? cpuUsagePercent
        : displayedCpuUsagePercent +
            STARTUP_SWEEP_RETURN_STEP;
  }

  if (
    displayedStorageUsedPercent >
      storageUsedPercent
  )
  {
    const uint8_t difference =
      displayedStorageUsedPercent -
      storageUsedPercent;

    displayedStorageUsedPercent =
      difference <= STARTUP_SWEEP_RETURN_STEP
        ? storageUsedPercent
        : displayedStorageUsedPercent -
            STARTUP_SWEEP_RETURN_STEP;
  }
  else if (
    displayedStorageUsedPercent <
      storageUsedPercent
  )
  {
    const uint8_t difference =
      storageUsedPercent -
      displayedStorageUsedPercent;

    displayedStorageUsedPercent =
      difference <= STARTUP_SWEEP_RETURN_STEP
        ? storageUsedPercent
        : displayedStorageUsedPercent +
            STARTUP_SWEEP_RETURN_STEP;
  }

  needsRender = true;

  if (
    displayedCpuUsagePercent ==
      cpuUsagePercent &&
    displayedStorageUsedPercent ==
      storageUsedPercent
  )
  {
    startupSweepActive = false;

    startupSweepReturning = false;

    lastGaugeAnimationAt = now;

    Serial.println(
      "MONITOR GAUGE SWEEP: COMPLETE"
    );
  }
}

void MonitorManager::render()
{
  if (leds == nullptr)
  {
    return;
  }

  /*
   * Only the HDD + CPU Gauge page is currently
   * implemented. Network and Service pages remain
   * reserved until their metrics are added.
   */
  currentPage =
    MonitorPage::SystemOverview;

  renderSystemOverview();

  leds->setEffectBrightness(
    255
  );

  leds->show();
}

void MonitorManager::renderSystemOverview()
{
  gaugeRenderer.render(
    leds->getHddLeds(),
    LED_COUNT_HDD,
    displayedStorageUsedPercent,
    getStorageColor(),
    getBackgroundColor()
  );

  gaugeRenderer.render(
    leds->getCpuLeds(),
    LED_COUNT_CPU,
    displayedCpuUsagePercent,
    getCpuColor(),
    getBackgroundColor()
  );

  for (
    uint16_t index = 0;
    index < LED_COUNT_INTAKE;
    index++
  )
  {
    leds->getIntakeLeds()[
      index
    ] = CRGB::Black;
  }

  for (
    uint16_t index = 0;
    index < LED_COUNT_EXHAUST;
    index++
  )
  {
    leds->getExhaustLeds()[
      index
    ] = CRGB::Black;
  }
}

void MonitorManager::renderPlaceholder(
  const CRGB& color
)
{
  for (
    uint16_t index = 0;
    index < LED_COUNT_HDD;
    index++
  )
  {
    leds->getHddLeds()[index] =
      color;
  }

  for (
    uint16_t index = 0;
    index < LED_COUNT_CPU;
    index++
  )
  {
    leds->getCpuLeds()[index] =
      color;
  }

  for (
    uint16_t index = 0;
    index < LED_COUNT_INTAKE;
    index++
  )
  {
    leds->getIntakeLeds()[index] =
      CRGB::Black;
  }

  for (
    uint16_t index = 0;
    index < LED_COUNT_EXHAUST;
    index++
  )
  {
    leds->getExhaustLeds()[index] =
      CRGB::Black;
  }
}

CRGB MonitorManager::
getBackgroundColor() const
{
  switch (baseGaugeColorIndex)
  {
    case 0:
      /*
       * A softer white base gives the full-brightness
       * red Gauge much clearer visual contrast.
       */
      return CRGB(
        135,
        135,
        135
      );

    case 1:
      return CRGB(
        10,
        10,
        18
      );

    case 2:
      return CRGB(
        28,
        0,
        38
      );

    case 3:
      return CRGB(
        180,
        220,
        255
      );

    case 4:
      return CRGB(
        45,
        24,
        2
      );

    case 5:
      return CRGB(
        2,
        12,
        8
      );

    default:
      return CRGB::White;
  }
}

CRGB MonitorManager::
getStorageColor() const
{
  switch (storageGaugeColorIndex)
  {
    case 0:
      return CRGB::Red;

    case 1:
      return CRGB::Orange;

    case 2:
      return CRGB::LimeGreen;

    case 3:
      return CRGB::Blue;

    case 4:
      return CRGB::Gold;

    case 5:
      return CRGB::Lime;

    default:
      return CRGB::Red;
  }
}

CRGB MonitorManager::
getCpuColor() const
{
  switch (cpuGaugeColorIndex)
  {
    case 0:
      return CRGB::Blue;

    case 1:
      return CRGB::Cyan;

    case 2:
      return CRGB::Magenta;

    case 3:
      return CRGB::Aqua;

    case 4:
      return CRGB::Red;

    case 5:
      return CRGB::Purple;

    default:
      return CRGB::Blue;
  }
}

CRGB MonitorManager::
getInactiveGroupColor() const
{
  CRGB color =
    getBackgroundColor();

  color.nscale8_video(
    80
  );

  return color;
}
