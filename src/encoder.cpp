#include <Arduino.h>

#include "configuration.h"
#include "encoder.h"

ENCODER_Rate EncoderRate;

bool Encoder::debounceReadPinDepressed()
{
    const unsigned long currentMillis = millis();
    if (currentMillis - this->previousDepressCheckMillis >= (unsigned int)debounceTime)
    {
        this->previousDepressCheckMillis = currentMillis;
        int btnEncoder = digitalRead(BTN_ENC);

        bool currentlyPressed = btnEncoder == LOW;

        if (currentlyPressed && !previousPressed)
        {
            this->previousPressed = true;
            return true;
        }
        else if (!currentlyPressed)
        {
            this->previousPressed = false;
        }
    }

    return false;
}

ENCODER_DiffState Encoder::analyze()
{
    const unsigned long now = millis();
    unsigned char newButton = 0;
    static signed char temp_diff = 0;

    ENCODER_DiffState tempDiffState = ENCODER_DIFF_NO;

    if (digitalRead(BTN_EN1) == HIGH)
    {
        newButton |= 0x01;
    }
    if (digitalRead(BTN_EN2) == HIGH)
    {
        newButton |= 0x02;
    }
    if (digitalRead(BTN_ENC) == LOW)
    {
        if ((now - this->lastClickUpdate) >= 1000)
        {
            this->lastClickUpdate = now + 1000;
            return ENCODER_DIFF_ENTER;
        }
        else
        {
            return ENCODER_DIFF_NO;
        }
    }

    if (newButton != this->lastEncoderBits)
    {
        switch (newButton)
        {
        case ENCODER_PHASE_0:
        {
            if (lastEncoderBits == ENCODER_PHASE_3)
                temp_diff++;
            else if (lastEncoderBits == ENCODER_PHASE_1)
                temp_diff--;
        }
        break;
        case ENCODER_PHASE_1:
        {
            if (lastEncoderBits == ENCODER_PHASE_0)
                temp_diff++;
            else if (lastEncoderBits == ENCODER_PHASE_2)
                temp_diff--;
        }
        break;
        case ENCODER_PHASE_2:
        {
            if (lastEncoderBits == ENCODER_PHASE_1)
                temp_diff++;
            else if (lastEncoderBits == ENCODER_PHASE_3)
                temp_diff--;
        }
        break;
        case ENCODER_PHASE_3:
        {
            if (lastEncoderBits == ENCODER_PHASE_2)
                temp_diff++;
            else if (lastEncoderBits == ENCODER_PHASE_0)
                temp_diff--;
        }
        break;
        }

        lastEncoderBits = newButton;
    }

    if (abs(temp_diff) >= ENCODER_PULSES_PER_STEP)
    {
        if (temp_diff > 0)
        {
            tempDiffState = ENCODER_DIFF_CW;
        }
        else
            tempDiffState = ENCODER_DIFF_CCW;

#ifdef ENCODER_RATE_MULTIPLIER
        constexpr int32_t encoderMultiplier = 1;
#else
        constexpr int32_t encoderMultiplier = 1;
#endif

        EncoderRate.encoderMoveValue = (temp_diff * encoderMultiplier) / (ENCODER_PULSES_PER_STEP);
        if (EncoderRate.encoderMoveValue < 0)
            EncoderRate.encoderMoveValue = -EncoderRate.encoderMoveValue;

        temp_diff = 0;
    }

    return tempDiffState;
}