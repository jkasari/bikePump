#include <Arduino.h>
#include <LiquidCrystal.h>

#define LCD_RS 3
#define LCD_E 5
#define LCD_D4 2
#define LCD_D5 4
#define LCD_D6 8
#define LCD_D7 9

class DisplayControl {

    public:

        DisplayControl();

        void displayNumber(float);

    private:
        LiquidCrystal LCD;
        uint8_t NumberCorners[8][8];
        uint8_t Numbers[10][4];
        
        void writeDigit(uint8_t, uint8_t);

        void writeCorner(uint8_t, uint8_t, uint8_t, uint8_t);

        void initNumbers();

        void initNumberCorners();
        
        void initLCDChars();
        

};