#include <Arduino.h>
#include <LiquidCrystal.h>

class DisplayControl {

    public:

        DisplayControl(LiquidCrystal*);

        void displayNumber(float);

    private:
        LiquidCrystal* LCDPointer;
        uint8_t NumberCorners[8][8];
        uint8_t Numbers[10][4];
        
        void writeDigit(uint8_t, uint8_t);

        void writeCorner(uint8_t, uint8_t, uint8_t, uint8_t);

        void initNumbers();

        void initNumberCorners();
        
        void initLCDChars();
        

};