#pragma once

#include <Arduino.h>
#include "Config.h"

class ButtonManager
{
public:
  void begin();
  void update();

  bool wasPressed();
  bool wasDoublePressed();
  bool wasTriplePressed();
  bool wasQuadruplePressed();

  // نگه‌داشتن ۱ تا کمتر از ۳ ثانیه
  bool wasLongPressed();

  // این سه رویداد فقط بعد از رهاکردن دکمه فعال می‌شوند
  bool wasSnooze24HoursPressed();
  bool wasSnooze30DaysPressed();
  bool wasSnooze6MonthsPressed();

  // رویدادهای راهنما هنگام نگه‌داشتن دکمه
  bool was24HourThresholdReached();
  bool was30DayThresholdReached();
  bool was6MonthThresholdReached();

  bool isCurrentlyPressed() const;

  unsigned long
  getCurrentHoldDuration() const;

  uint8_t getCurrentPressNumber() const;

  void suppressCurrentPressRelease();

private:
  bool lastReading = HIGH;
  bool stableState = HIGH;

  bool pressedEvent = false;
  bool doublePressedEvent = false;
  bool triplePressedEvent = false;
  bool quadruplePressedEvent = false;
  bool longPressedEvent = false;

  bool snooze24HoursEvent = false;
  bool snooze30DaysEvent = false;
  bool snooze6MonthsEvent = false;

  bool threshold24HourEvent = false;
  bool threshold30DayEvent = false;
  bool threshold6MonthEvent = false;

  bool threshold24HourTriggered = false;
  bool threshold30DayTriggered = false;
  bool threshold6MonthTriggered = false;

  uint8_t clickCount = 0;

  bool suppressReleaseEvent = false;

  unsigned long lastDebounceTime = 0;
  unsigned long pressStartTime = 0;
  unsigned long lastClickReleasedAt = 0;

  static constexpr unsigned long DOUBLE_CLICK_MS =
    600UL;

  static constexpr unsigned long LONG_PRESS_MS =
    1000UL;

  static constexpr unsigned long SNOOZE_24H_MS =
    3000UL;

  static constexpr unsigned long SNOOZE_30D_MS =
    6000UL;

  static constexpr unsigned long SNOOZE_6M_MS =
    10000UL;
};
