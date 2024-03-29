#include <Arduino.h>
#include "helper.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
AirGate::AirGate(uint8_t Pin) {
    pinMode(Pin, OUTPUT);
    this->Pin = Pin;
}

bool AirGate::checkGate() {
    uint32_t val = 0;
    if (!closed) {
        // If the gate is open, check to see how long.
        if (millis() - timeOpenned > openDuration) {
            flipGate(false); // If its longer then the set time, close it. 
            val = openDuration;
        }
    }
    return val; // Returns true if the gate is closed.
}

void AirGate::turnGateOn() {
    flipGate(true);
}

void AirGate::turnGateOff() {
    flipGate(false);
}

void AirGate::turnGateOn(float diff, float currentPSI) {
    float psiDiff = oldPSI > currentPSI ? oldPSI - currentPSI : currentPSI - oldPSI; 
    flowRate = .2;
    //if (openDuration != 0) {
    //    flowRate = (psiDiff * 1000) / openDuration;
    //} else {
    //    flowRate = .1;
    //}
    uint32_t milliSeconds = uint32_t(diff*flowRate);
    if (milliSeconds < MIN_OPEN_TIME) {
        milliSeconds = MIN_OPEN_TIME; // If the number is to small set it to the minimum time.
    }
    oldPSI = currentPSI;
    openDuration = milliSeconds;
    //Serial.print("open duration: ");
    //Serial.print(openDuration);
    //Serial.print("  PSI diff: ");
    //Serial.print(psiDiff);
    //Serial.print("  pressure diff: ");
    //Serial.print(diff);
    //Serial.print("  flow rate: ");
    //Serial.print(flowRate);
    //Serial.print("  Old PSI: ");
    //Serial.print(oldPSI);
    //Serial.print("  current PSI: ");
    //Serial.println(currentPSI);
    flipGate(true); // Open the gate. 
}

void AirGate::flipGate(bool highOrLow) {
    closed = !highOrLow; // True means the gate is open, so closed would be false. 
    if (highOrLow) {
        timeOpenned = millis(); // record the exact time the gate was openned. 
    }
    digitalWrite(Pin, int(highOrLow)); // set the gate pin to on or off. 
}

void AirGate::resetGate(float currentPSI) {
    oldPSI = currentPSI;
    openDuration = 0;
}

bool AirGate::isClosed() {
    return closed;
}

Button::Button(uint8_t Pin) { 
    this->Pin = Pin; 
    pinMode(Pin, INPUT_PULLUP);
}

uint32_t Button::hasBeenPressed() {
    uint32_t value = 0;
    if (currentlyPressed()) {
        TimePressed++; // If currently pressed, increment the time pressed. 
    }
    if (!currentlyPressed() && TimePressed > 0) {
        value = TimePressed; // If the button is not pressed but has a record of being pressed, reset the time pressed and return how long it was pressed for. 
        TimePressed = 0;
    }
    return value;
}

bool Button::currentlyPressed() {
    return digitalRead(Pin) == LOW; // Return if the pin has been pulled up. 
}

MainController::MainController() : 

    Button_1(BUTTON1),
    Button_2(BUTTON2),
    Button_3(BUTTON3),
    Button_4(BUTTON4),
    Button_5(BUTTON5),
    Button_6(BUTTON6),
    BigNums(NASA),
    Gate_1(GATE_1),
    Gate_2(GATE_2) {
        pinMode(LCD_RS, OUTPUT);
        pinMode(LCD_E, OUTPUT);
        pinMode(LCD_D4, OUTPUT);
        pinMode(LCD_D5, OUTPUT);
        pinMode(LCD_D6, OUTPUT);
        pinMode(LCD_D7, OUTPUT);
    }

void MainController::begin() {
  lcd.begin(16, 2);
}

bool MainController::gatesClosed() {
    bool ans = false;
    if (Gate_2.isClosed() && Gate_1.isClosed()) {
        settleTimer++; // start counting how long the gates have been closed for. 
        if (millis() - settleTimer > SETTLE_TIME) {
            ans = true; // If its closed and settled, return value gets set to true. 
        }
    } else {
        settleTimer = millis(); // If either gate is open, keep the settle time current. 
    }
    return ans;
}

bool MainController::isStable(float pressure) {
    bool stable = false;
    recordPressure(pressure); // record the new pressure to the recorded pressures.
    float ave = getAveragePressure(); // Get the average of the recorded pressures. 
    float diff = ave > pressure ? ave - pressure : pressure - ave; // Get the diff between the new pressure and average pressures.
    if (!diff) { diff = 0; } // For start up edge case when there is no ave pressure. 
    if (gatesClosed() && diff < STABLE_TOLERANCE) {
        stable = true; // If the gates are closed and the diff is within tolerance, set return value to true.
    }
    Serial.println("checked gates");
    //Serial.print("Stable : "+String(stable));
    checkGates(pressure);
    return stable;
}

// CHANGED HERE
void MainController::smartMode(float pressure) {
    touchTarget(); // check the buttons for any changes in the target. 
    if (isStable(pressure)) { // Only continue if we have a stable pressure.
        if (!Manual) { // If we were not just in manual mode. 
            float diff = pressure > Target ? pressure - Target : Target - pressure;
            if (Target > pressure || diff > .3) {
                adjustGates(Target, pressure, diff);
            } 
        } else { // If we just came out of manual mode, set the target to the current pressure. This happens when you first connect a tire to the pump. 
            Manual = false;
            Target = round(pressure); // Round to the nearest non float number. 
            Gate_1.resetGate(pressure);
            Gate_2.resetGate(pressure);
        }
    }
    checkGates(pressure);
}

void MainController::manualMode(float pressure) {
    //if (isStable(pressure)) { // check to make sure the pressure is stable before continuing
        Manual = true; // Record that we are in manual mode.
        Target = pressure; // Set the target to whatever the current pressure is.
    //}
    if (Button_2.currentlyPressed()) { // The gates being different numbers from the buttons is not a bug, I just have the buttons swapped on the pcb. 
        Gate_1.turnGateOn();
    } else {
        Gate_1.turnGateOff();
    }
    if (Button_1.currentlyPressed()) { // The gates being different numbers from the buttons is not a bug, I just have the buttons swapped on the pcb. 
        Gate_2.turnGateOn();
    } else {
        Gate_2.turnGateOff();
    }
}

bool MainController::checkGates(float currentPsi) {
    uint32_t gate2OpenTime = Gate_2.checkGate();
    uint32_t gate1OpenTime = Gate_1.checkGate();
    float oldPsi = OldPressures[STORED_PRESSURE_COUNT - 1];
    float diff = oldPsi < currentPsi ? currentPsi -  oldPsi : oldPsi - currentPsi;
    if (gate2OpenTime > 0) {
       gate2Bias = gate2OpenTime / diff; 
    }
    if (gate1OpenTime > 0) {
        gate1Bias = gate1OpenTime / diff;
    }
    Serial.print("Gate 1 Bias :  ");
    Serial.println(gate1Bias);
    Serial.print("Gate 2 Bias :  ");
    Serial.println(gate2Bias);
    Serial.println("");
}

void MainController::adjustGates(float targetPSI, float currentPSI, float diff) {
    if (targetPSI > currentPSI) {
        //Serial.print("gate 1: ");
        Gate_1.turnGateOn(diff, currentPSI);
    } else {
        //Serial.print("gate 2: ");
        Gate_2.turnGateOn(diff, currentPSI);
    }
}

//void MainController::adjustGates(float target, float current) {
//    float diff = target - current; 
//    //Serial.println("  target - current = diff : "+String(target)+" - "+String(current)+" = "+String(diff));
//    if (diff > 0) { // calcAndOpenGate needs a positive number, so we mulitple negative differences by -1. 
//        calcAndOpenGate(true, diff);
//    } else {
//        calcAndOpenGate(false, diff * -1);
//    }
//}


//CHANGED HERE
//void MainController::calcAndOpenGate(bool gateOneorTwo, float diff) {
//    //uint32_t waitTime = diff * (-2000/(diff+1)+2000);
//    float waitTime = diff * 50; // Take the difference and multiple it by 200 to give us the milliseconds to be open for.  
//    waitTime = uint32_t(round(waitTime));
//    // Must be in milli seconds
//    if (gateOneorTwo) {// choose which gate to open. 
//        Gate_1.turnGateOn( waitTime);
//    } else {
//        Gate_2.turnGateOn(waitTime);
//    }
//}

void MainController::touchTarget() {
    // Brute force baby!!!
    if (Button_1.hasBeenPressed()) {
        Target -= .5;
        Serial.print("button1 Pressed");
    } else if (Button_2.hasBeenPressed()) {
        Target += .5;
    }
    if (Button_3.hasBeenPressed()) {
        Target = 17;
    }
    if (Button_4.hasBeenPressed()) {
        Target = 50;
    }
    if (Button_5.hasBeenPressed()) {
        Target = 45;
    }
    if (Button_6.hasBeenPressed()) {
        Target = 23.5;
    }
    if (Target > 50) {
        Target = 50;
    }
    if (0 > Target) {
        Target = 0;
    }
}

void MainController::recordPressure(float newPressure) {
    for (int i = STORED_PRESSURE_COUNT - 1; i > 0; --i) {
        OldPressures[i] = OldPressures[i-1]; // Move all the pressures down one. 
    }
    OldPressures[0] = newPressure; // Set the first pressure to the new value. 
}

float MainController::getAveragePressure() {
    float sum = 0;
    float nonZeroReadings = 0;
    for (int i = 0; i < STORED_PRESSURE_COUNT; ++i) {
        if (OldPressures[i] != 0) {
            nonZeroReadings += 1;
            sum += OldPressures[i] / 10;
        }
    }
    if (nonZeroReadings == 0) {
        return 0; // Avoid dividing by zero. 
    }
    return (sum / nonZeroReadings) * 10;
}

float MainController::getTarget() {
    return Target;
}

void MainController::displayTargetAndCurrent(float targetPSI, float currentPSI) {
    if (targetPSI != oldTargetPSI || currentPSI != oldCurrentPSI) {
      lcd.clear();
      BigNums.printShortFloat(targetPSI, 1, 2, 0, 0);
      BigNums.printShortFloat(currentPSI, 1, 2, 9, 0);
      oldTargetPSI = targetPSI;
      oldCurrentPSI = currentPSI;
    }

}
