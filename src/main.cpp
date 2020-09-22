#include <Arduino.h>
#include <AutoPID.h>
#include <string.h>

#include "configuration.h"
#include "encoder.h"
#include "lcd.h"
#include "thermometer.h"

int encoderPos = 1;               // Current encoder position
int encoder0PinALast;             // Used to decode rotory encoder, last value
int encoder0PinNow;               // Used to decode rotory encoder, current value
unsigned long previousMillis = 0; // Previous
unsigned long currentMillis;      // Current
const long interval = 1000;       // / 3;   // Update on 1/3 of a second.

const long tempReadInterval = TEMP_READ_INTERVAL;
long lcdUpdateInterval = 1000 / 3;

double setTemperature = 50.0;

double inputTemperature = 0;
double outputTemperature = 0;

double defaultKp = DEFAULT_Kp;
double defaultKi = DEFAULT_Ki;
double defaultKd = DEFAULT_Kd;

bool heating = false;

Encoder encoder;
LcdHelper lcd(lcdUpdateInterval, &heating, &inputTemperature, &setTemperature);
Thermometer thermometer(TEMP_READ_INTERVAL);
AutoPID pid(&inputTemperature, &setTemperature, &outputTemperature, 0, 255, defaultKp, defaultKi, defaultKd);

void setup()
{
  Serial.begin(9600); // open the serial port at 9600 bps:

  pinMode(BTN_EN1, INPUT);     // Set BTN_EN1 as an unput, half of the encoder
  digitalWrite(BTN_EN1, HIGH); // turn on pullup resistors
  pinMode(BTN_EN2, INPUT);     // Set BTN_EN2 as an unput, second half of the encoder
  digitalWrite(BTN_EN2, HIGH); // turn on pullup resistors
  pinMode(BTN_ENC, INPUT);     // Set BTN_ENC as an unput, encoder button
  digitalWrite(BTN_ENC, HIGH); // turn on pullup resistors

  //turn the PID on
  pid.setBangBang(4);

  // set up the LCD's number of columns and rows:
  lcd.init();
}

void loop()
{
  // If we're heating insure that the PID is running.
  if (heating)
  {
    pid.run();
  }

  // If our
  if (thermometer.isTemperatureUpdated())
  {
    float currentTemperature = thermometer.getCurrentTemperature();
    char currentTempAsChar[4], currentTemperatureNotice[20];

    dtostrf(currentTemperature, 4, 1, currentTempAsChar);
    snprintf(currentTemperatureNotice, sizeof(currentTemperatureNotice), "Current %sC", currentTempAsChar);
    lcd.setLine(1, currentTemperatureNotice);

    inputTemperature = currentTemperature;
  }

  // Read the encoder and update encoderPos
  // encoder0PinNow = digitalRead(BTN_EN1);
  // if ((encoder0PinALast == LOW) && (encoder0PinNow == HIGH))
  // {
  //   if (digitalRead(BTN_EN2) == HIGH)
  //   {
  //     setTemperature++;
  //   }
  //   else
  //   {
  //     setTemperature--;
  //   }
  // }
  // encoder0PinALast = encoder0PinNow;

  //read the encoder button status
  bool encoderPressedStatus = encoder.debounceReadPinDepressed();
  if (encoderPressedStatus)
  {
    Serial.println("debounced pressed");
  }
  if (encoderPressedStatus && !heating)
  {
    lcd.setLine(3, "Heating...");
    heating = true;
  }
  else if (encoderPressedStatus && heating)
  {
    lcd.setLine(3, "Cooling down...");
    pid.stop();
    heating = false;
  }

  if (!heating && inputTemperature <= 30)
  {
    lcd.setLine(3, "Ready...");
  }

  // Last thing to do is update our display.
  lcd.update();
}
