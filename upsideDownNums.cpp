#include <Arduino.h>
#include "upsideDownNums.h"

#define TOLERANCE 10


DisplayControl::DisplayControl() :
    LCD(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7) { 
    pinMode(LCD_RS, OUTPUT);
    pinMode(LCD_E, OUTPUT);
    pinMode(LCD_D4, OUTPUT);
    pinMode(LCD_D5, OUTPUT);
    pinMode(LCD_D6, OUTPUT);
    pinMode(LCD_D7, OUTPUT);
    initNumbers(); 
}

void DisplayControl::displayNumber(float num) {
    LCD.clear();
    uint32_t temp = round(num*10);
    uint8_t index = 0;
    writeCorner(2, 0, 2, 3);
    while (temp > 0) {    
        uint32_t digit = temp % 10;
        writeDigit(index, digit);
        if (index == 0) {
            index += 3;
        } else {
            index += 2;
        }
        temp /= 10;
    }
}

void DisplayControl::writeDigit(uint8_t index, uint8_t digit) {
    writeCorner(1+index, 1, digit, 0);
    writeCorner(0+index, 1, digit, 1);
    writeCorner(1+index, 0, digit, 2);
    writeCorner(0+index, 0, digit, 3);
}

void DisplayControl::writeCorner(uint8_t x, uint8_t y, uint8_t numIndex, uint8_t cornerIndex) {
    uint8_t corner = Numbers[numIndex][cornerIndex];
    LCD.setCursor(x, y);
    if (corner >= 0 && 8 > corner) {
        LCD.write(corner);
    }
}

void DisplayControl::initLCDChars() {
    for (int i = 0; i < 8; ++i) {
        LCD.createChar(i, NumberCorners[i]);
    }
}

void DisplayControl::initNumbers() {
    initNumberCorners();

    Numbers[0][0] = 5;
    Numbers[0][1] = 7;
    Numbers[0][2] = 6;
    Numbers[0][3] = 5;

    Numbers[1][0] = -1;
    Numbers[1][1] = 1;
    Numbers[1][2] = -1;
    Numbers[1][3] = 1;

    Numbers[2][0] = 0;
    Numbers[2][1] = 4;
    Numbers[2][2] = 5;
    Numbers[2][3] = 2;

    Numbers[3][0] = 0;
    Numbers[3][1] = 4;
    Numbers[3][2] = 2;
    Numbers[3][3] = 4;

    Numbers[4][0] = 5;
    Numbers[4][1] = 1;
    Numbers[4][2] = 0;
    Numbers[4][3] = 1;

    Numbers[5][0] = 6;
    Numbers[5][1] = 0;
    Numbers[5][2] = 2;
    Numbers[5][3] = 4;

    Numbers[6][0] = 5;
    Numbers[6][1] = -1;
    Numbers[6][2] = 6;
    Numbers[6][3] = 4;

    Numbers[7][0] = 0;
    Numbers[7][1] = 5;
    Numbers[7][2] = 5;
    Numbers[7][3] = -1;

    Numbers[8][0] = 3;
    Numbers[8][1] = 4;
    Numbers[8][2] = 3;
    Numbers[8][3] = 4;

    Numbers[9][0] = 3;
    Numbers[9][1] = 7;
    Numbers[9][2] = -1;
    Numbers[9][3] = 5;

}

void DisplayControl::initNumberCorners() {
    NumberCorners[0][0] = B00000,
    NumberCorners[0][1] = B00000,
    NumberCorners[0][2] = B00000,
    NumberCorners[0][3] = B00000,
    NumberCorners[0][4] = B00000,
    NumberCorners[0][5] = B00000,
    NumberCorners[0][6] = B01111,
    NumberCorners[0][7] = B11111,

    NumberCorners[1][0] = B00011,
    NumberCorners[1][1] = B00011,
    NumberCorners[1][2] = B00011,
    NumberCorners[1][3] = B00011,
    NumberCorners[1][4] = B00011,
    NumberCorners[1][5] = B00011,
    NumberCorners[1][6] = B00011,
    NumberCorners[1][7] = B00011,

    NumberCorners[2][0] = B11111,
    NumberCorners[2][1] = B01111,
    NumberCorners[2][2] = B00000,
    NumberCorners[2][3] = B00000,
    NumberCorners[2][4] = B00000,
    NumberCorners[2][5] = B00000,
    NumberCorners[2][6] = B00000,
    NumberCorners[2][7] = B00000,

    NumberCorners[3][0] = B11100,
    NumberCorners[3][1] = B01110,
    NumberCorners[3][2] = B00111,
    NumberCorners[3][3] = B00011,
    NumberCorners[3][4] = B00011,
    NumberCorners[3][5] = B00111,
    NumberCorners[3][6] = B01110,
    NumberCorners[3][7] = B11100,

    NumberCorners[4][0] = B00111,
    NumberCorners[4][1] = B01110,
    NumberCorners[4][2] = B11100,
    NumberCorners[4][3] = B11000,
    NumberCorners[4][4] = B11000,
    NumberCorners[4][5] = B11100,
    NumberCorners[4][6] = B01110,
    NumberCorners[4][7] = B00111,

    NumberCorners[5][0] = B00011,
    NumberCorners[5][1] = B00011,
    NumberCorners[5][2] = B00110,
    NumberCorners[5][3] = B00110,
    NumberCorners[5][4] = B01100,
    NumberCorners[5][5] = B01100,
    NumberCorners[5][6] = B11000,
    NumberCorners[5][7] = B11000,

    NumberCorners[6][0] = B11100,
    NumberCorners[6][1] = B01110,
    NumberCorners[6][2] = B00111,
    NumberCorners[6][3] = B00011,
    NumberCorners[6][4] = B00011,
    NumberCorners[6][5] = B00011,
    NumberCorners[6][6] = B00011,
    NumberCorners[6][7] = B00011,

    NumberCorners[7][0] = B11000,
    NumberCorners[7][1] = B11000,
    NumberCorners[7][2] = B11000,
    NumberCorners[7][3] = B11000,
    NumberCorners[7][4] = B11000,
    NumberCorners[7][5] = B11100,
    NumberCorners[7][6] = B01110,
    NumberCorners[7][7] = B00111,
    
    initLCDChars();
}