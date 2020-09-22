#pragma once

class Thermometer
{
private:
    float readThermistor();
    float convertResistenceToTemperature(float resistence);

public:
    float readCurrentTemperature();
};