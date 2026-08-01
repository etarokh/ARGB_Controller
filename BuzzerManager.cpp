#include "BuzzerManager.h"

void BuzzerManager::begin()
{
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  previousCriticalMask = 0;
  mutedCriticalMask = 0;
  buzzing = false;

  buzzStartedAt = 0;
  lastBuzzStartedAt = 0;
}

void BuzzerManager::update(uint8_t criticalMask)
{
  const unsigned long now = millis();

  // Critical پاک‌شده از فهرست Mute هم حذف می‌شود.
  // اگر بعداً دوباره برگردد، Critical جدید محسوب خواهد شد.
  mutedCriticalMask &= criticalMask;

  const uint8_t newCriticalMask =
    criticalMask &
    static_cast<uint8_t>(~previousCriticalMask);

  const uint8_t audibleCriticalMask =
    criticalMask &
    static_cast<uint8_t>(~mutedCriticalMask);

  if (criticalMask == 0)
  {
    if (buzzing)
    {
      stopBuzz();
    }

    previousCriticalMask = 0;
    mutedCriticalMask = 0;
    return;
  }

  // هر Critical جدید که Mute نشده، فوراً بوق می‌زند.
  if (
    (newCriticalMask &
      static_cast<uint8_t>(~mutedCriticalMask)) != 0
  )
  {
    if (!buzzing)
    {
      startBuzz();
    }
  }

  // پایان بوق دوثانیه‌ای
  if (
    buzzing &&
    now - buzzStartedAt >= BUZZ_DURATION_MS
  )
  {
    stopBuzz();
  }

  // تکرار هر پنج دقیقه تا وقتی Critical شنیدنی وجود دارد.
  if (
    audibleCriticalMask != 0 &&
    !buzzing &&
    now - lastBuzzStartedAt >= REPEAT_INTERVAL_MS
  )
  {
    startBuzz();
  }

  // اگر همه Criticalهای فعلی Mute شدند، بوق قطع شود.
  if (
    audibleCriticalMask == 0 &&
    buzzing
  )
  {
    stopBuzz();
  }

  previousCriticalMask = criticalMask;
}

void BuzzerManager::mute(uint8_t currentCriticalMask)
{
  mutedCriticalMask |= currentCriticalMask;

  const uint8_t remainingAudibleMask =
    previousCriticalMask &
    static_cast<uint8_t>(~mutedCriticalMask);

  if (
    remainingAudibleMask == 0 &&
    buzzing
  )
  {
    stopBuzz();
  }

  Serial.print(
    "BUZZER: Muted Critical mask 0x"
  );

  Serial.println(
    currentCriticalMask,
    HEX
  );
}

// سازگاری موقت با کد قبلی
void BuzzerManager::update(bool criticalActive)
{
  update(
    static_cast<uint8_t>(
      criticalActive ? 0x01U : 0x00U
    )
  );
}

// سازگاری موقت با کد قبلی
void BuzzerManager::mute()
{
  mute(previousCriticalMask);
}

void BuzzerManager::beep(unsigned long durationMs)
{
  digitalWrite(PIN_BUZZER, HIGH);
  delay(durationMs);
  digitalWrite(PIN_BUZZER, LOW);

  Serial.println(
    "BUZZER: Confirmation beep"
  );
}

bool BuzzerManager::isMuted() const
{
  return mutedCriticalMask != 0;
}

bool BuzzerManager::isBuzzing() const
{
  return buzzing;
}

uint8_t BuzzerManager::getMutedCriticalMask() const
{
  return mutedCriticalMask;
}

void BuzzerManager::startBuzz()
{
  digitalWrite(PIN_BUZZER, HIGH);

  buzzing = true;
  buzzStartedAt = millis();
  lastBuzzStartedAt = buzzStartedAt;

  Serial.println(
    "BUZZER: Critical alarm"
  );
}

void BuzzerManager::stopBuzz()
{
  digitalWrite(PIN_BUZZER, LOW);
  buzzing = false;
}
