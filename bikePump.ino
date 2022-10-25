#include "helper.h"
#include "upsideDownNums.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal.h>

#define PSI_CONSTANT 0.015625

Adafruit_ADS1115 ADS;
DisplayControl Display;
MainController Controller;

void setup() {
    Serial.begin(115200);
    ADS.begin();
    ADS.setGain(GAIN_SIXTEEN);
}

void loop() {
    float currentPSI = getReading();
    Controller.testingFunction();
    Display.displayNumber(currentPSI);
    delay(1000);
}


float getReading() {
    return ADS.readADC_Differential_0_1() * PSI_CONSTANT + .40;
}