#include "helper.h"
#include "upsideDownNums.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal.h>

#define LCD_RS 3
#define LCD_E 5
#define LCD_D4 2
#define LCD_D5 4
#define LCD_D6 8
#define LCD_D7 9
#define PSI_CONSTANT 0.015625


MainController Controller;

void setup() {
    Serial.begin(115200);
}

void loop() {
    Controller.testingFunction();
}