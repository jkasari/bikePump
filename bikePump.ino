#include "helper.h"
#include "upsideDownNums.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal.h>

#define PSI_CONSTANT 0.015625
#define LOW_LIMIT 9.99

Adafruit_ADS1115 ADS;
DisplayControl Display;
MainController Controller;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    ADS.begin();
    ADS.setGain(GAIN_SIXTEEN);
}

void loop() {
    float currentPSI = getPressure();
    if (currentPSI > LOW_LIMIT) {
        Controller.smartMode(currentPSI);
    } else {
        Controller.manualMode(currentPSI);
    }
    Display.displayNumber(currentPSI);
    //Serial.println("Target: "+String(Controller.getTarget()));
    Controller.checkGates();
}


float getPressure() {
    float pressure = ADS.readADC_Differential_0_1();
    if (pressure < 0) {
        pressure = 0;
    }
    return pressure * PSI_CONSTANT;
}