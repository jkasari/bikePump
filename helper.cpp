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

void AirGate::turnGateOn(uint8_t seconds) {
    openDuration = seconds * 1000;
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
    return digitalRead(Port) == HIGH;
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
    if (!isStable(pressure)) {
        Serial.println("Not Stable!!!");
    }
}

bool MainController::gatesClosed() {
    return Gate_1.isClosed() && Gate_2.isClosed();
}

bool MainController::isStable(float pressure) {
    if (millis() - StableCheckTime > SETTLE_TIME) {
        StableCheckTime = millis();
        float diff = pressure > OldPressure ? pressure - OldPressure : OldPressure - pressure;
        OldPressure = pressure;
        if (gatesClosed() && diff < STABLE_TOLERANCE) {
            Stable = false;
        }
    }
}

void MainController::checkGates() {
    Gate_1.checkGate();
    Gate_2.checkGate(); 
}