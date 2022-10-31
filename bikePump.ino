#include "helper.h"
#include "upsideDownNums.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal.h>

#define PSI_CONSTANT 0.015625
#define LOW_LIMIT 1.99 // The numbers that decideds what mode to go into. 

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
    float currentPSI = getPressure(); // Get the current pressure
    if (Controller.gatesClosed() && currentPSI < LOW_LIMIT) { // If the gates are closed and the current psi is below the low limit, go into smart mode. 
        Controller.manualMode(currentPSI);
    } else { // Otherwise, go into manual mode. This happens when no tire is attached to the pump. 
        Controller.smartMode(currentPSI);
    }
    Display.displayTargetAndCurrent(Controller.getTarget(), currentPSI);
    //Serial.println("Target: "+String(Controller.getTarget()));
    Controller.checkGates();
    //Display.displayNumber(currentPSI);
    //Serial.println(" Target/current : "+String(Controller.getTarget())+"/"+String(currentPSI));
}


float getPressure() {
    float pressure = ADS.readADC_Differential_0_1();
    if (pressure < 0) {
        pressure = 0; // Doesn't let any negative pressures come through
    }
    return pressure * PSI_CONSTANT;
}