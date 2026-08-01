#include "KnockRouter.h"

void KnockRouter::begin(
  KnockManager* knockManager,
  ModeManager* systemModeManager
)
{
  knock = knockManager;
  modeManager = systemModeManager;
}

void KnockRouter::update()
{
  if (
    knock == nullptr ||
    modeManager == nullptr
  )
  {
    return;
  }

  knock->update();

  if (!knock->wasDetected())
  {
    return;
  }

  Serial.println(
    "KNOCK: NEXT MODE"
  );

  modeManager->nextMode();
}
