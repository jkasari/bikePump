#include "helper.h"
#include "upsideDownNums.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal.h>

#define PSI_CONSTANT 0.015625

DisplayControl Display;
MainController Controller;

void setup() {
    Serial.begin(115200);
}

void loop() {
    Controller.testingFunction();
    Display.displayNumber(66.6);
    delay(1000);
}