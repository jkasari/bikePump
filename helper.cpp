#include "helper.h"

AirGate::AirGate(uint8_t Pin) {
    pinMode(Pin, OUTPUT);
    this->Pin = Pin;
}

bool AirGate::checkGate() {
    if (!closed) {
        if (millis() - timeOpenned > openDuration) {
            flipGate(false);
        }
    }
    return closed;
}

void AirGate::turnGateOn() {
    flipGate(true);
}

void AirGate::turnGateOff() {
    flipGate(false);
}

void AirGate::turnGateOn(uint32_t milliSeconds) {
    if (milliSeconds < MIN_OPEN_TIME) {
        milliSeconds = MIN_OPEN_TIME:
    }
    openDuration = milliSeconds;
    flipGate(true);
}

void AirGate::flipGate(bool highOrLow) {
    closed = !highOrLow;
    if (highOrLow) {
        timeOpenned = millis();
    }
    digitalWrite(Pin, int(highOrLow));
}

bool AirGate::isClosed() {
    return closed;
}

Button::Button(uint8_t Port) { 
    this->Port = Port; 
    pinMode(Port, INPUT_PULLUP);
}

uint32_t Button::hasBeenPressed() {
    uint32_t value = 0;
    if (isPressed()) {
        Count++;
    }
    if (!isPressed() && Count > 0) {
        value = Count;
        Count = 0;
    }
    return value;
}

bool Button::isPressed() {
    return digitalRead(Port) == LOW;
}

MainController::MainController() : 
    Button_1(BUTTON1),
    Button_2(BUTTON2),
    Button_3(BUTTON3),
    Button_4(BUTTON4),
    Button_5(BUTTON5),
    Button_6(BUTTON6),
    Gate_1(GATE_1),
    Gate_2(GATE_2) {}

void MainController::testingFunction(float pressure) {
    isStable(pressure);
}

bool MainController::gatesClosed() {
    bool ans = false;
    if (Gate_2.isClosed() && Gate_1.isClosed()) {
        settleTimer++;
        if (millis() - settleTimer > SETTLE_TIME) {
            ans = true;
        }
    } else {
        settleTimer = millis();
    }
    return ans;
}

bool MainController::isStable(float pressure) {
        StableCheckTime = millis();
        bool stable = false;
        recordPressure(pressure);
        float ave = getAveragePressure();
        float diff = ave > pressure ? ave - pressure : pressure - ave;
        if (!diff) { diff = 0; }
        if (gatesClosed() && diff < STABLE_TOLERANCE) {
            stable = true;
        }
    //Serial.print("Stable : "+String(stable));
    return stable;
}

void MainController::smartMode(float pressure) {
    touchTarget();
    if (isStable(pressure)) {
        if (!Manual) {
            float diff = pressure > Target ? pressure - Target : Target - pressure;
            if (diff > TOLERANCE) {
                adjustGates(Target, pressure);
            } 
        } else {
            Manual = false;
            Target = round(pressure);
        }
    }
}

void MainController::manualMode(float pressure) {
    if (isStable(pressure)) {
        Manual = true;
        Target = pressure;
    }
    if (Button_1.isPressed()) {
        calcAndOpenGate(false, 1);
    } else if (Button_2.isPressed()) {
        calcAndOpenGate(true, 1);
    }
}

bool MainController::checkGates() {
    Gate_2.checkGate();
    Gate_1.checkGate();
}

void MainController::adjustGates(float target, float current) {
    float diff = target - current;
    Serial.println("  target - current = diff : "+String(target)+" - "+String(current)+" = "+String(diff));
    if (diff > 0) {
        calcAndOpenGate(true, diff);
    } else {
        calcAndOpenGate(false, diff * -1);
    }
}

void MainController::calcAndOpenGate(bool gateNum, float diff) {
    //uint32_t waitTime = diff * (-2000/(diff+1)+2000);
    float waitTime = diff * 100;
    waitTime = uint32_t(round(waitTime));
    // Must be in milli seconds
    if (gateNum) {
        Gate_1.turnGateOn( waitTime);
    } else {
        Gate_2.turnGateOn(waitTime);
    }
}

void MainController::touchTarget() {
    if (Button_1.hasBeenPressed()) {
        Target -= .5;
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
        OldPressures[i] = OldPressures[i-1];
    }
    OldPressures[0] = newPressure;
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
        return 0;   
    }
    return (sum / nonZeroReadings) * 10;
}

float MainController::getTarget() {
    return Target;
}