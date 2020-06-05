#ifndef LATCHS_H
#define LATCHS_H
#include "Hardwares.h"

class Latchs : public Hardwares
{
private:
    Q_OBJECT
    Voltage pins[LATCH_PIN_NUM];
    Counter<Latchs> c;
    int status;

public:
    Latchs(QString latchName=nullptr);
    void setPinVoltage(MicroCom::Pins pin, Voltage value);
    Voltage getPinVoltage(MicroCom::Pins pin);
    void handlePinVolChanges(MicroCom::Pins pin, Voltage value);
    static int howMany(){return Counter<Latchs>::howMany();}
};

#endif // LATCHS_H
