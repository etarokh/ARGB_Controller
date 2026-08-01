#pragma once

#include <Arduino.h>

#include "KnockManager.h"
#include "ModeManager.h"

class KnockRouter
{
public:
  void begin(
    KnockManager* knockManager,
    ModeManager* systemModeManager
  );

  void update();

private:
  KnockManager* knock = nullptr;
  ModeManager* modeManager = nullptr;
};
