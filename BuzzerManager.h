#pragma once

#include <Arduino.h>
#include "Config.h"

class BuzzerManager
{
public:
  void begin();

  // نسخه جدید: هر بیت نماینده یک بخش Critical است.
  void update(uint8_t criticalMask);
  void mute(uint8_t currentCriticalMask);

  // سازگاری موقت با کد فعلی ARGB_Controller.ino
  void update(bool criticalActive);
  void mute();

  void beep(unsigned long durationMs = 100UL);

  bool isMuted() const;
  bool isBuzzing() const;

  uint8_t getMutedCriticalMask() const;

private:
  uint8_t previousCriticalMask = 0;
  uint8_t mutedCriticalMask = 0;

  bool buzzing = false;

  unsigned long buzzStartedAt = 0;
  unsigned long lastBuzzStartedAt = 0;

  static constexpr unsigned long BUZZ_DURATION_MS =
    2000UL;

  static constexpr unsigned long REPEAT_INTERVAL_MS =
    5UL * 60UL * 1000UL;

  void startBuzz();
  void stopBuzz();
};
