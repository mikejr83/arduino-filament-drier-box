#pragma once

#define LCD_COLS            20
#define LCD_ROWS            4
#define LCD_UPDATE_INTERVAL 100

#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

#define BTN_ENC 7
#define BTN_EN2 8
#define BTN_EN1 9

#define BEEPER  6

#define THERMISTOR_PULLUP_RESISTOR_VALUE    9800    //4700
#define THERMISTOR_PIN                      A0
#define THERMISTOR_READ_SAMPLES             5
#define THERMISTOR_NOMINAL                  100000  // resistance at 25 degrees C
#define TEMPERATURE_NOMINAL                 25      // temp. for nominal resistance (almost always 25 C)
#define BCOEFFICIENT                        3950    // The beta coefficient of the thermistor (usually 3000-4000)

#define TEMP_READ_INTERVAL                  1000    // Time in sec

#define MIN_TEMP                            28
#define MAX_TEMP                            80

#define HEATER_PIN                          10

#define DEFAULT_Kp 21.73
#define DEFAULT_Ki 1.54
#define DEFAULT_Kd 76.55

#define DEBUG_TEMPERATURE_READING false
#define DEBUG_LCD_LINE_PRINT false

// Change values more rapidly when the encoder is rotated faster
// #define ENCODER_RATE_MULTIPLIER
#ifdef ENCODER_RATE_MULTIPLIER
  #define ENCODER_10X_STEPS_PER_SEC   30  // (steps/s) Encoder rate for 10x speed
  #define ENCODER_100X_STEPS_PER_SEC  80  // (steps/s) Encoder rate for 100x speed
#endif