#include <Arduino.h>

#include "LedManager.h"
#include "ButtonManager.h"
#include "BuzzerManager.h"
#include "KnockManager.h"
#include "EffectManager.h"
#include "DecorativeEffectManager.h"
#include "ModeManager.h"
#include "CommandParser.h"

LedManager leds;
ButtonManager button;
BuzzerManager buzzer;
KnockManager knock;
EffectManager effects;
DecorativeEffectManager decorativeEffects;
ModeManager modeManager;
CommandParser commandParser;

void printDecorativeStatus()
{
  Serial.println();
  Serial.println(
    "Decorative status:"
  );

  Serial.print(
    "Effect: "
  );

  Serial.println(
    decorativeEffects.getEffectName()
  );

  Serial.print(
    "Palette: "
  );

  Serial.println(
    decorativeEffects.getPaletteName()
  );

  Serial.print(
    "Speed: "
  );

  Serial.print(
    decorativeEffects.getSpeed()
  );

  Serial.println("%");
}

void playSnoozeAnimation(
  uint8_t beepCount,
  uint8_t flashCount,
  const CRGB& flashColor
)
{
  for (uint8_t i = 0; i < beepCount; i++)
  {
    buzzer.beep(100);

    if (i + 1 < beepCount)
    {
      delay(120);
    }
  }

  for (uint8_t i = 0; i < flashCount; i++)
  {
    leds.setAll(flashColor);
    leds.show();
    delay(220);

    leds.clear();
    delay(180);
  }
}

void setup()
{
  Serial.begin(115200);

  leds.begin();
  button.begin();
  buzzer.begin();
  knock.begin();

  effects.begin(
    &leds
  );

  effects.setDefaultColor(
    CRGB::Blue
  );

  commandParser.begin(
    &effects,
    &modeManager
  );

  decorativeEffects.begin(
    &leds
  );

  decorativeEffects.setColor(
    CRGB::Blue
  );

  decorativeEffects.setPalette(
    DecorativePaletteType::Ocean
  );

  decorativeEffects.setSpeed(
    35
  );

  decorativeEffects.setEffect(
    DecorativeEffectType::StaticColor
  );

  modeManager.begin(
    &leds,
    &effects,
    &decorativeEffects
  );

  modeManager.setMode(
    SystemMode::Decorative
  );

  Serial.println();
  Serial.println(
    "Decorative control test"
  );

  Serial.println();
  Serial.println(
    "Short button press:"
  );

  Serial.println(
    "Next decorative effect"
  );

  Serial.println();
  Serial.println(
    "Double button press:"
  );

  Serial.println(
    "Double-click detection test"
  );

  Serial.println();
  Serial.println(
    "Long button press:"
  );

  Serial.println(
    "Next decorative palette"
  );

  Serial.println();
  Serial.println(
    "Hold 1-3 sec:"
  );

  Serial.println(
    "Next decorative palette"
  );

  Serial.println();
  Serial.println(
    "Hold 3-6 sec:"
  );

  Serial.println(
    "24-hour alert snooze"
  );

  Serial.println();
  Serial.println(
    "Hold 6-10 sec:"
  );

  Serial.println(
    "30-day alert snooze"
  );

  Serial.println();
  Serial.println(
    "Hold 10+ sec:"
  );

  Serial.println(
    "6-month alert snooze"
  );

  Serial.println();
  Serial.println(
    "Knock:"
  );

  Serial.println(
    "Decoration <-> Off"
  );

  Serial.println();
  Serial.println(
    "Effect order:"
  );

  Serial.println(
    "1. Static Color"
  );

  Serial.println(
    "2. Breathing"
  );

  Serial.println(
    "3. Rainbow"
  );

  Serial.println(
    "4. Color Cycle"
  );

  Serial.println(
    "5. Aurora"
  );

  Serial.println(
    "6. Ocean Wave"
  );

  printDecorativeStatus();
}

void loop()
{
  commandParser.update();

  button.update();
  knock.update();

  bool threshold24HourReached =
    button.was24HourThresholdReached();

  bool threshold30DayReached =
    button.was30DayThresholdReached();

  bool threshold6MonthReached =
    button.was6MonthThresholdReached();

  bool snooze24HoursPress =
    button.wasSnooze24HoursPressed();

  bool snooze30DaysPress =
    button.wasSnooze30DaysPressed();

  bool snooze6MonthsPress =
    button.wasSnooze6MonthsPressed();

  bool longButtonPress =
    button.wasLongPressed();

  bool doubleButtonPress =
    button.wasDoublePressed();

  bool quadrupleButtonPress =
    button.wasQuadruplePressed();

  bool shortButtonPress =
    button.wasPressed();

  if (threshold24HourReached)
  {
    buzzer.beep(70);

    Serial.println(
      "BUTTON HOLD: 24-HOUR THRESHOLD"
    );
  }

  if (threshold30DayReached)
  {
    buzzer.beep(70);
    delay(90);
    buzzer.beep(70);

    Serial.println(
      "BUTTON HOLD: 30-DAY THRESHOLD"
    );
  }

  if (threshold6MonthReached)
  {
    buzzer.beep(70);
    delay(90);
    buzzer.beep(70);
    delay(90);
    buzzer.beep(70);

    Serial.println(
      "BUTTON HOLD: 6-MONTH THRESHOLD"
    );
  }

  if (shortButtonPress)
  {
    Serial.println(
      "BUTTON: SINGLE"
    );
  }

  if (doubleButtonPress)
  {
    Serial.println(
      "BUTTON: DOUBLE"
    );
  }

  if (quadrupleButtonPress)
  {
    buzzer.beep(100);
    delay(100);
    buzzer.beep(250);
    Serial.println(
      "BUTTON: FOUR"
    );
  }

  if (longButtonPress)
  {
    Serial.println(
      "BUTTON: LONG"
    );
  }

  if (snooze24HoursPress)
  {
    Serial.println(
      "BUTTON: SNOOZE 24 HOURS"
    );
  }

  if (snooze30DaysPress)
  {
    Serial.println(
      "BUTTON: SNOOZE 30 DAYS"
    );
  }

  if (snooze6MonthsPress)
  {
    Serial.println(
      "BUTTON: SNOOZE 6 MONTHS"
    );
  }

  bool anyAlertActive =
    effects.hasActiveAlert();

  if (
    snooze6MonthsPress &&
    anyAlertActive
  )
  {
    buzzer.mute(
      effects.getCriticalAlertMask()
    );

    modeManager.snoozeAlertsFor6Months();

    playSnoozeAnimation(
      3,
      10,
      CRGB::Red
    );
  }
  else if (
    snooze30DaysPress &&
    anyAlertActive
  )
  {
    buzzer.mute(
      effects.getCriticalAlertMask()
    );

    modeManager.snoozeAlertsFor30Days();

    playSnoozeAnimation(
      2,
      6,
      CRGB::Green
    );
  }
  else if (
    snooze24HoursPress &&
    anyAlertActive
  )
  {
    buzzer.mute(
      effects.getCriticalAlertMask()
    );

    modeManager.snoozeAlertsFor24Hours();

    playSnoozeAnimation(
      1,
      3,
      CRGB::Green
    );
  }
  else if (quadrupleButtonPress)
  {
    Serial.println(
      "CLEAR_ACTIVE"
    );
  }
  else if (
    effects.hasCriticalAlert() &&
    shortButtonPress
  )
  {
    buzzer.mute(
      effects.getCriticalAlertMask()
    );

    Serial.println(
      "BUZZER: Current Critical alerts muted"
    );
  }
  else if (longButtonPress)
  {
    decorativeEffects.nextPalette();

    printDecorativeStatus();
  }
  else if (shortButtonPress)
  {
    decorativeEffects.nextEffect();

    printDecorativeStatus();
  }

  uint8_t audibleCriticalMask =
    effects.getHardwareCriticalAlertMask();

  buzzer.update(
    audibleCriticalMask
  );

  if (knock.wasDetected())
  {
    modeManager.nextMode();
  }

  modeManager.update();

  delay(5);
}
