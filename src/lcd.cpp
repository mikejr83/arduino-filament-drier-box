#include <LiquidCrystal.h>
#include <Wire.h>

#include "configuration.h"
#include "lcd.h"

LcdHelper::LcdHelper() : defaultLcd(RS, EN, D4, D5, D6, D7)
{
}

void LcdHelper::init()
{
    this->defaultLcd.begin(LCD_COLS, LCD_ROWS);
}

void LcdHelper::println(int row, char *str)
{
    this->defaultLcd.setCursor(0, row);
    char s[20];
    snprintf(s, sizeof(s), "%-20s", str);
    this->defaultLcd.print(s);
}