#include <Arduino.h>

#include <AutoPID.h>

#include "configuration.h"
#include "heater.h"

#define HEATER_PIN_ON 0
#define HEATER_PIN_OFF 255

Heater::Heater(double *currentTemperature, double *targetTemperature)
    : currentTemperature(currentTemperature), targetTemperature(targetTemperature), pidR(currentTemperature, targetTemperature, &activateHeater, 10000, 10, 5, 1)
{
}

bool Heater::getHeating()
{
    return this->heating;
}

void Heater::setHeating(bool heating)
{
    // if (!this->heating && heating)
    // {
    //     this->pidR.reset();
    // }
    this->heating = heating;
}

double Heater::getTargetTemperature()
{
    return *this->targetTemperature;
}

void Heater::run()
{
    if (!this->heating)
    {
        this->activateHeater = false;
        // this->pidR.stop();
        //       analogWrite(HEATER_PIN, HEATER_PIN_OFF);
        digitalWrite(HEATER_PIN, LOW);
        return;
    }
    
    unsigned long currentTime = millis();
    if ((currentTime - this->previousUpdate) >= 1000)
    {
        this->previousUpdate = currentTime;

        // this->pidR.run();


        if (*currentTemperature < *targetTemperature)
        {
            Serial.println("Light the burners!");
            analogWrite(HEATER_PIN, HEATER_PIN_ON);
            // digitalWrite(HEATER_PIN, HIGH);
        }
        else
        {
            Serial.println("Let it chill");
            analogWrite(HEATER_PIN, HEATER_PIN_OFF);
            // digitalWrite(HEATER_PIN, LOW);
        }
    }

    digitalWrite(HEATER_PIN, activateHeater);
}
