#include <Arduino.h>

#include <PID_v1.h>
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
const long interval = 1000 / 3;   // Update on 1/3 of a second.
double setTemperature = 50;

double inputTemperature = 0;
double outputTemperature = 0;

double defaultKp = DEFAULT_Kp;
double defaultKi = DEFAULT_Ki;
double defaultKd = DEFAULT_Kd;

bool heating = false;

Encoder encoder;
LcdHelper lcd;
Thermometer thermometer;
PID pid(&inputTemperature, &outputTemperature, &setTemperature, defaultKp, defaultKi, defaultKd, DIRECT);

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
  pid.SetMode(AUTOMATIC);

  // set up the LCD's number of columns and rows:
  lcd.init();
  // Print a message to the LCD.
  lcd.println(0, "Hot Box v1.0");
  lcd.println(3, "Ready...");
}

void loop()
{
  // Read the encoder and update encoderPos
  encoder0PinNow = digitalRead(BTN_EN1);
  if ((encoder0PinALast == LOW) && (encoder0PinNow == HIGH))
  {
    if (digitalRead(BTN_EN2) == HIGH)
    {
      setTemperature++;
    }
    else
    {
      setTemperature--;
    }
  }
  encoder0PinALast = encoder0PinNow;

  // if (encoderPos > 10)
  // {
  //   digitalWrite(BEEPER, HIGH);
  // }
  // else
  // {
  //   digitalWrite(BEEPER, LOW);
  // }

  //read the encoder button status
  bool encoderPressedStatus = encoder.debounceReadPinDepressed();
  if (encoderPressedStatus)
  {
    Serial.println("debounced pressed");
  }
  if (encoderPressedStatus && !heating)
  {
    lcd.println(3, "Heating");
    heating = true;
  }
  else if (encoderPressedStatus && heating)
  {
    lcd.println(3, "Cooling down...");
    heating = false;
  }

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    inputTemperature = thermometer.readCurrentTemperature();

    char currentTempAsChar[4];
    char currentTemperatureNotice[32];
    dtostrf(inputTemperature, 3, 1, currentTempAsChar);
    snprintf(currentTemperatureNotice, sizeof(currentTemperatureNotice), "Current Temp %sC", currentTempAsChar);
    char setTemperatureNotice[64];

    snprintf(setTemperatureNotice, sizeof(setTemperatureNotice), "Set Temp %iC", (int)setTemperature);

    if (inputTemperature > 28)
    {
      //pid.Compute();

      // Serial.println(outputTemperature);
    }

    // lcd.setCursor(0, 1);
    lcd.println(1, currentTemperatureNotice);
    // lcd.setCursor(0, 2);
    lcd.println(2, setTemperatureNotice);
  }
}

void readRequestedTemperature()
{
  // Read the encoder and update encoderPos
  encoder0PinNow = digitalRead(BTN_EN1);
  if ((encoder0PinALast == LOW) && (encoder0PinNow == HIGH))
  {
    if (digitalRead(BTN_EN2) == LOW)
    {
      setTemperature++;
    }
    else
    {
      setTemperature--;
    }
  }
  encoder0PinALast = encoder0PinNow;
}
