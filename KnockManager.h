#pragma once

#include <Arduino.h>
#include "Config.h"

class KnockManager {
public:
    void begin();
    void update();

    bool wasDetected();

    int getRawValue() const;

private:
    int rawValue = 0;

    bool knockEvent = false;
    bool knockArmed = true;

    unsigned long lastKnockTime = 0;
};
