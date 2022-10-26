#include <Arduino.h>

#define BUTTON_NUM 6
#define BUTTON1 A3
#define BUTTON2 10
#define BUTTON3 11
#define BUTTON4 A2
#define BUTTON5 A1
#define BUTTON6 12
#define GATE_NUM 2
#define GATE_1 6
#define GATE_2 7
#define STABLE_TOLERANCE .1
#define STORED_PRESSURE_COUNT 15
#define TOLERANCE .5


/**
 * @brief A class that controls the basic functions of a gate.
 * This keeps track of info about the gate such as:
 * How long it's been open or closed for.
 * Automatic turn off given a set time.
 */

class AirGate {

    public:
        AirGate(uint8_t);

        // MUST CALL in a loop for the auto turn off to happen
        // Returns true if the gate is closed, false if not
        bool checkGate();

        // A function that turns the gate on with no time limit
        void turnGateOn();
        
        // Turns a gate off.
        void turnGateOff();

        // Takes in the number of seconds you want the gate to be turned on for. 
        void turnGateOn(uint8_t);

        // Returns true if the gate is closed. 
        bool isClosed();

    private:
        uint8_t Pin;
        bool closed = true;
        uint32_t openDuration = 0;
        uint32_t timeOpenned = 0;
        void flipGate(bool);

};

/**
 * The button class takes in a port number to read.
 * Call the check function in your loop to check if the button was pressed.
 * use the result to decide what to do based on how long the button was held down for.
 */

class Button {

  public:
  // Takes a port number to read
  Button(uint8_t);

  // Returns a count of how long the button was pressed for only after it's been pressed and released. 
  uint32_t hasBeenPressed();

  // Returns true when the button is pressed and false when released.
  bool isPressed();

  private:
  uint32_t Count;
  uint8_t Port;
};

/**
 * Talks to the buttons, gates and pressure reader. 
 * Has functions that return usable data from these objects.
 */

class MainController {

    public:

        MainController();
        
        void testingFunction(float);

        bool isStable(float);

        void checkGates();

        void smartMode(float);

        void manualMode(float); 

        float getTarget();

    private:
        Button Button_1;
        Button Button_2;
        Button Button_3;    
        Button Button_4;    
        Button Button_5;    
        Button Button_6;    
        AirGate Gate_1;
        AirGate Gate_2;
        float OldPressures[STORED_PRESSURE_COUNT];
        uint32_t StableCheckTime = 0;
        bool Manual = true;
        float Target = 0.0;

        bool gatesClosed();

        void adjustGates(float, float);

        void calcAndOpenGate(bool, float);

        void touchTarget();

        void recordPressure(float);

        float getAveragePressure();
};