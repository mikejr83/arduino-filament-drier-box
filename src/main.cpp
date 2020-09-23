#include <Arduino.h>
#include <AutoPID.h>
#include <string.h>

#include "configuration.h"
#include "encoder.h"
#include "heater.h"
#include "lcd.h"
#include "thermometer.h"

int encoderPos = 1;               // Current encoder position
int encoder0PinALast;             // Used to decode rotory encoder, last value
int encoder0PinNow;               // Used to decode rotory encoder, current value
unsigned long previousMillis = 0; // Previous
unsigned long currentMillis;      // Current
const long interval = 1000;       // / 3;   // Update on 1/3 of a second.

const long tempReadInterval = TEMP_READ_INTERVAL;

double setTemperature = 35.0;
double inputTemperature = 0;
double outputTemperature = 0;

float encoderOffset = 0;

double defaultKp = DEFAULT_Kp;
double defaultKi = DEFAULT_Ki;
double defaultKd = DEFAULT_Kd;

bool heating = false;

Encoder encoder;
LcdHelper lcd(LCD_UPDATE_INTERVAL, &heating, &inputTemperature, &setTemperature);
Thermometer thermometer(TEMP_READ_INTERVAL);
Heater heater(&inputTemperature, &setTemperature);

void setup()
{
  Serial.begin(9600); // open the serial port at 9600 bps:

  pinMode(BTN_EN1, INPUT);     // Set BTN_EN1 as an unput, half of the encoder
  digitalWrite(BTN_EN1, HIGH); // turn on pullup resistors
  pinMode(BTN_EN2, INPUT);     // Set BTN_EN2 as an unput, second half of the encoder
  digitalWrite(BTN_EN2, HIGH); // turn on pullup resistors
  pinMode(BTN_ENC, INPUT);     // Set BTN_ENC as an unput, encoder button
  digitalWrite(BTN_ENC, HIGH); // turn on pullup resistors

  // set up the LCD's number of columns and rows:
  lcd.init();
}

void loop()
{
  heater.run();

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

  ENCODER_DiffState diffState = encoder.analyze();
  
  switch (diffState)
  {
  case ENCODER_DIFF_CCW:
    setTemperature -= 1;
    Serial.println(setTemperature);
    break;

  case ENCODER_DIFF_CW:
    setTemperature += 1;
    Serial.println(setTemperature);
    break;

  default:
    break;
  }

  if (diffState == ENCODER_DIFF_ENTER)
  {
    //Serial.println("PUSHED");
  }

  currentMillis = millis();
  if (currentMillis - previousMillis >= 800)
  {
    previousMillis = currentMillis;
    char setTempAsChar[4], setTemperatureNotice[20];
    dtostrf(setTemperature, 4, 1, setTempAsChar);
    snprintf(setTemperatureNotice, sizeof(setTemperatureNotice), "Set to %sC", setTempAsChar);
    lcd.setLine(2, setTemperatureNotice);
  }

  //read the encoder button status
  bool encoderPressedStatus = encoder.debounceReadPinDepressed();
  if (encoderPressedStatus)
  {
    Serial.println("debounced pressed");
  }
  if (encoderPressedStatus && !heater.getHeating())
  {
    lcd.setLine(3, "Heating...");
    heater.setHeating(true);
  }
  else if (encoderPressedStatus && heater.getHeating())
  {
    lcd.setLine(3, "Cooling down...");
    heater.setHeating(false);
  }

  if (!heater.getHeating() && inputTemperature <= 30)
  {
    lcd.setLine(3, "Ready...");
  }

  // Last thing to do is update our display.
  lcd.update();
}
