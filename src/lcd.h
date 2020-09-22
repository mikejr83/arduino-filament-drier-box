#pragma once

#include <LiquidCrystal.h>

#include "configuration.h"

class LcdHelper
{
private:
    LiquidCrystal defaultLcd;

    char lines[LCD_ROWS][LCD_COLS];
    bool changed[LCD_ROWS];

    long updateInterval;
    unsigned long lastUpdateTime;

    double *currentTemperature;
    bool *heating;
    double *setTemperature;

public:
    LcdHelper(long updateInterval, bool *heating, double *currentTemperature, double *setTemperature);
    void init();
    void setLine(int row, char *str);
    void update();
    void println(int row, const char str[]);
};