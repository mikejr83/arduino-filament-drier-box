#include <AutoPID.h>

class Heater
{
private:
    bool heating = false;
    bool activateHeater = false;

    double *targetTemperature;
    double *currentTemperature;

    unsigned long previousUpdate;

    AutoPIDRelay pidR;

public:
    Heater(double *currentTemperature, double *targetTemperature);

    void setHeating(bool heating);
    bool getHeating();

    void setTargetTemperature(double targetTemperature);
    double getTargetTemperature();

    void run();
};