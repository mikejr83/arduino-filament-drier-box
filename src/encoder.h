class Encoder
{
private:
    unsigned long previousMillis = 0;
    int debounceTime = 200;
    bool previousPressed = false;

public:
    // void readEncoder();
    bool debounceReadPinDepressed();
};