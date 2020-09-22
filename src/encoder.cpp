#include <Arduino.h>

#include "configuration.h"
#include "encoder.h"

bool Encoder::debounceReadPinDepressed()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= (unsigned int)debounceTime)
    {
        previousMillis = currentMillis;
        int btnEncoder = digitalRead(BTN_ENC);

        bool currentlyPressed = btnEncoder == LOW;

        if (currentlyPressed && !previousPressed)
        {
            previousPressed = true;
            return true;
        }
        else if (!currentlyPressed)
        {
            previousPressed = false;
        }
    }

    return false;
}