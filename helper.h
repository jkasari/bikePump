#include <Arduino.h>

#define BUTTON1 A3
#define BUTTON2 10
#define BUTTON3 11
#define BUTTON4 A2
#define BUTTON5 A1
#define BUTTON6 12
#define GATE_1 7
#define GATE_2 6
#define SETTLE_TIME 50 // The wait time in milliseconds before the mainController checks the pressure
#define STABLE_TOLERANCE .001 // The limit for the differnce between the average of the stored readings an the current reading
#define STORED_PRESSURE_COUNT 15 // How many readings are stored to later have the average calculated.
#define TOLERANCE .3 // Limit on how close the program tries to get to the target pressure
#define MIN_OPEN_TIME 50 // Minimum time the gates can be open for 


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
        void turnGateOn(uint32_t);

        // Returns true if the gate is closed. 
        bool isClosed();

    private:
        uint8_t Pin; // What arduino pin controls the gate
        bool closed = true;
        uint32_t openDuration = 0; // the set time for a gate to be openned
        uint32_t timeOpenned = 0; // Measure how long a gate has been open for
        // turn the gate on or off directly. 
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
  uint32_t TimePressed; // keep track of how long the button has been pressed
  uint8_t Port; // What pin on the arduino reads the button
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

        bool checkGates();
        
        bool gatesClosed();

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
        uint32_t settleTimer = 100;
        float OldPressures[STORED_PRESSURE_COUNT];
        uint32_t StableCheckTime = 0;
        bool Manual = true;
        float Target = 0.0;

        void adjustGates(float, float);

        void calcAndOpenGate(bool, float);

        void touchTarget();

        void recordPressure(float);

        float getAveragePressure();
};