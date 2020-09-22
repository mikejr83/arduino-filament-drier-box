#pragma once

class Thermometer
{
private:
    float currentTemperature;
    long interval;
    unsigned long lastReadingTime;
    float readThermistor();
    float convertResistenceToTemperature(float resistence);

public:
    float readTemperature();
    float getCurrentTemperature();
    bool isTemperatureUpdated();
    Thermometer(long interval);
};