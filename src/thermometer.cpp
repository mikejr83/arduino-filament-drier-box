#include <Arduino.h>

#include "configuration.h"
#include "thermometer.h"

Thermometer::Thermometer(long interval)
{
    this->interval = interval;
    this->currentTemperature = INT32_MIN;
    this->lastReadingTime = 0;
}

float Thermometer::readTemperature()
{
    float resistence, temperature;

    resistence = this->readThermistor();

#if DEBUG_TEMPERATURE_READING
    Serial.print("Resistence: ");
    Serial.println(resistence);
#endif

    temperature = this->convertResistenceToTemperature(resistence);

#if DEBUG_TEMPERATURE_READING
    Serial.print("Temperature: ");
    Serial.println(temperature);
#endif

    return temperature;
}

bool Thermometer::isTemperatureUpdated()
{
    unsigned long currentTime = millis();
    if ((currentTime - this->lastReadingTime) >= this->interval)
    {
        this->lastReadingTime = currentTime;
        this->currentTemperature = this->readTemperature();
        return true;
    }
    else
    {
        return false;
    }
}

float Thermometer::getCurrentTemperature()
{
    return this->currentTemperature;
}

float Thermometer::readThermistor()
{
    int samples[THERMISTOR_READ_SAMPLES];
    int i = 0;

    for (i = 0; i < THERMISTOR_READ_SAMPLES; i++)
    {
        samples[i] = analogRead(THERMISTOR_PIN);
        delay(10);
    }

    // average all the samples out
    float total = 0;
    for (i = 0; i < THERMISTOR_READ_SAMPLES; i++)
    {
        total += samples[i];
    }
    float average = total / THERMISTOR_READ_SAMPLES;

    float reading = (1023 / average) - 1;                 // (1023/ADC - 1)
    reading = THERMISTOR_PULLUP_RESISTOR_VALUE / reading; // 4.7K / (1023/ADC - 1)

    return reading;
}

float Thermometer::convertResistenceToTemperature(float resistence)
{
    float steinhart;
    steinhart = resistence / THERMISTOR_NOMINAL;       // (R/Ro)
    steinhart = log(steinhart);                        // ln(R/Ro)
    steinhart = steinhart / BCOEFFICIENT;              // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                       // Invert
    steinhart -= 273.15;                               // convert to C

    return steinhart;
}