
#ifndef ampel_h
#define ampel_h

enum e_state {GREEN, ORANGE_GREEN, ORANGE, ORANGE_RED, RED};
enum e_mode {AUTO, ZUG};

class Ampel
{
public:
    Ampel(int _buttonPin, int _greenPin, int _orangePin, int _redPin);
    
    void setNextState();
    bool readButton();
    void writeLeds();
    char getState();
    void setState(e_state state);
    
private:
    int buttonPin, greenPin, orangePin, redPin;
    
    int buttonState;             // the current reading from the input pin
    int lastButtonState;         // the previous reading from the input pin
    long lastDebounceTime;       // the last time the output pin was toggled
    
    enum e_state previous_state;
    enum e_state state;
    enum e_mode  mode;

    long timeout;

    void setNextTimeoutState();    
};

#endif