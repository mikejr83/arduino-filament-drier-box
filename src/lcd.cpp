#include <LiquidCrystal.h>
#include <Wire.h>

#include "configuration.h"
#include "lcd.h"

LcdHelper::LcdHelper(long updateInterval, bool *heating, double *currentTemperature, double *setTemperature)
    : updateInterval(updateInterval), heating(heating), currentTemperature(currentTemperature), setTemperature(setTemperature), defaultLcd(RS, EN, D4, D5, D6, D7), lastUpdateTime(0)
{
    for (int i = 0; i < LCD_ROWS; i++)
    {
        this->changed[i] = false;
    }
}

void LcdHelper::init()
{
    this->defaultLcd.begin(LCD_COLS, LCD_ROWS);
    this->setLine(0, "Hot Box v1.0");
    this->setLine(3, "Ready...");
}

void LcdHelper::update()
{
    unsigned long currentTime = millis();
    if ((currentTime - lastUpdateTime) >= (unsigned long)this->updateInterval)
    {
        this->lastUpdateTime = currentTime;

        int i;
        for (i = 0; i < LCD_ROWS; i++)
        {
            if (this->changed[i])
            {
                this->changed[i] = false;
                this->println(i, lines[i]);
            }
        }
    }
}

void LcdHelper::setLine(int row, char *str)
{
    strcpy(this->lines[row], str);
    this->changed[row] = true;
}

void LcdHelper::println(int row, const char str[])
{
    this->defaultLcd.setCursor(0, row);
    char s[20];
    snprintf(s, sizeof(s), "%-20s", str);

#if DEBUG_LCD_LINE_PRINT
    Serial.print("Update LCD line ");
    Serial.print(row);
    Serial.print(": ");
    Serial.println(s);
#endif

    this->defaultLcd.print(s);
}
