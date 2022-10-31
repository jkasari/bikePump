#include <Arduino.h>
#include <LiquidCrystal.h>

#define LCD_RS 3
#define LCD_E 5
#define LCD_D4 2
#define LCD_D5 4
#define LCD_D6 8
#define LCD_D7 9

/**
 * @brief Controls the LCD
 * Sense the lcd screen is installed upside down, this class needs to be able to display full size numbers upside down. 
 * 
 */
class DisplayControl {

    public:

        DisplayControl();

        // Takes a index and float to display at that index. 
        void displayNumber(uint8_t, float);
        
        // Takes the target and current pressures and displays them on the screen. 
        void displayTargetAndCurrent(float, float);

    private:
        LiquidCrystal LCD;
        uint8_t NumberCorners[8][8]; // Hold the 8 unique characters stored on the LCD.
        uint8_t Numbers[10][4]; // Holds the 10 cominations of those charaters that make up all 10 digits. 
        
        // takes an index and a single digit to write on the display at that index. 
        void writeDigit(uint8_t, uint8_t);

        // Takes the x ,y location of the corner. The digit the corner is from and the costum char number. 
        void writeCorner(uint8_t, uint8_t, uint8_t, uint8_t);

        // Builds all 10 digits out of the 8 chars. 
        void initNumbers();

        // Builds the 8 unique chars that make up each corner for the digits. 
        void initNumberCorners();
        
        // Actually saves those 8 custom chars to the LCD. 
        void initLCDChars();
        

};