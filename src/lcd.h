#pragma once

#include <LiquidCrystal.h>

#include "configuration.h"

class LcdHelper
{
private:
    LiquidCrystal defaultLcd;

public:
    LcdHelper();
    void init();
    void println(int row, char *str);
};