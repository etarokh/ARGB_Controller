#include "KnockManager.h"

void KnockManager::begin() {
    pinMode(PIN_KNOCK, INPUT);

    rawValue = 0;
    knockEvent = false;
    knockArmed = true;
    lastKnockTime = 0;
}

void KnockManager::update() {
    rawValue = analogRead(PIN_KNOCK);

    const unsigned long now = millis();

    if (
        knockArmed &&
        rawValue > KNOCK_THRESHOLD &&
        now - lastKnockTime >= KNOCK_LOCKOUT_MS
    ) {
        knockEvent = true;
        knockArmed = false;
        lastKnockTime = now;
    }

    // Re-arm only after the signal has clearly dropped.
    if (
        !knockArmed &&
        rawValue < (KNOCK_THRESHOLD / 2)
    ) {
        knockArmed = true;
    }
}

bool KnockManager::wasDetected() {
    if (!knockEvent) {
        return false;
    }

    knockEvent = false;
    return true;
}

int KnockManager::getRawValue() const {
    return rawValue;
}
