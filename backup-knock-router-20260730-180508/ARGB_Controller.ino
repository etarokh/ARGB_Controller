#include <Arduino.h>

#include "LedManager.h"
#include "ButtonManager.h"
#include "ButtonRouter.h"
#include "BuzzerManager.h"
#include "KnockManager.h"
#include "EffectManager.h"
#include "DecorativeEffectManager.h"
#include "ModeManager.h"
#include "CommandParser.h"

LedManager leds;
ButtonManager button;
ButtonRouter buttonRouter;
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

  buttonRouter.begin(
    &button,
    &buzzer,
    &leds,
    &effects,
    &decorativeEffects,
    &modeManager
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

  knock.update();

  buttonRouter.update();

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
