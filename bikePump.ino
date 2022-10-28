#include "helper.h"
#include "upsideDownNums.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal.h>

#define PSI_CONSTANT 0.015625
#define LOW_LIMIT 1.99

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
    Serial.print("Target = : "+String(Controller.getTarget())+"   ");
    Serial.print("Current = : "+String(currentPSI)+"   ");
    if (currentPSI > LOW_LIMIT) {
        Controller.smartMode(currentPSI);
    } else {
        Controller.manualMode(currentPSI);
    }
    Display.displayTargetAndCurrent(Controller.getTarget(), currentPSI);
    //Serial.println("Target: "+String(Controller.getTarget()));
    Controller.checkGates();
    Serial.println("");
    delay(5);
}


float getPressure() {
    float pressure = ADS.readADC_Differential_0_1();
    if (pressure < 0) {
        pressure = 0;
    }
    return pressure * PSI_CONSTANT;
}