#include "helper.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#define PSI_CONSTANT 0.015625
#define LOW_LIMIT 1.99 // The numbers that decideds what mode to go into. 
#define SCREEN_RATE 150

Adafruit_ADS1115 ADS;
MainController Controller;
uint32_t timer = 0;
uint32_t lastStablePressure = 0;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    ADS.begin();
    ADS.setGain(GAIN_SIXTEEN);
    Controller.begin();
}

void loop() {
    //Serial.println("Line 22");
    float currentPSI = getPressure(); // Get the current pressure
    //Serial.println("Line 24");
    if (Controller.isStable(currentPSI)) {
    ///Serial.println("Line 26");
      lastStablePressure = currentPSI;
    //Serial.println("Line 28");
    }
    if (lastStablePressure < LOW_LIMIT) { // If the gates are closed and the current psi is below the low limit, go into smart mode. 
    //Serial.println("Line 31");
        Controller.manualMode(currentPSI);
    //Serial.println("Line 33");
    } else { // Otherwise, go into manual mode. This happens when no tire is attached to the pump. 
        Controller.smartMode(currentPSI);
    //Serial.println("Line 36");
    }
    if (millis() - timer > SCREEN_RATE) {
    //Serial.println("Line 39");
        Controller.displayTargetAndCurrent(Controller.getTarget(), currentPSI);
    //Serial.println("Line 41");
        timer = millis();
    }
    //Serial.println("Line 44");
    //Serial.println("Target: "+String(Controller.getTarget()));
    Controller.checkGates();
    //Display.displayNumber(currentPSI);
    //printOutData(currentPSI, Controller.getTarget(), Controller.isStable(currentPSI));
}


float getPressure() {
    float pressure = ADS.readADC_Differential_0_1();
    if (pressure < 0) {
        pressure = 0; // Doesn't let any negative pressures come through
    }
    return pressure * PSI_CONSTANT;
}

void printOutData(float currentPsi, float target, bool stable) {
    Serial.print("Target: ");
    Serial.print(target);
    Serial.print(" | ");

    Serial.print("Current: ");
    Serial.print(currentPsi);
    Serial.print(" | ");
    
    Serial.print("Is Stable: ");
    Serial.println(stable);
}
