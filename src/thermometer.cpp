#include <Arduino.h>

#include "configuration.h"
#include "thermometer.h"

float Thermometer::readCurrentTemperature()
{
    float resistence, temperature;

    resistence = this->readThermistor();

    // Serial.print("Resistence: ");
    // Serial.println(resistence);

    temperature = this->convertResistenceToTemperature(resistence);

    return temperature;
}

float Thermometer::readThermistor()
{
    int samples[THERMISTOR_READ_SAMPLES];
    int i = 0;

    for (i = 0; i < THERMISTOR_READ_SAMPLES; i++)
    {
        // Serial.print("i=");
        // Serial.print(i);
        // Serial.print(" read: ");
        samples[i] = analogRead(THERMISTOR_PIN);
        // Serial.println(samples[i]);
        delay(10);
    }

    // average all the samples out
    float total = 0;
    for (i = 0; i < THERMISTOR_READ_SAMPLES; i++)
    {
        total += samples[i];
    }
    float average = total / THERMISTOR_READ_SAMPLES;

    // Serial.print("Total of readings: ");
    // Serial.print(total);
    // Serial.print(" Average read: ");
    // Serial.println(average);

    float reading = (1023 / average) - 1;                 // (1023/ADC - 1)
    reading = THERMISTOR_PULLUP_RESISTOR_VALUE / reading; // 4.7K / (1023/ADC - 1)

    // Serial.print("Calculated reading: ");
    // Serial.println(reading);

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