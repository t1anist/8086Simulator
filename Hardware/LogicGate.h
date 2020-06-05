#ifndef NAND_H
#define NAND_H
#include "Hardwares.h"


//特别的，对于非门（NOT）而言，使用LG_I1，LG_I2要变为低电平
class LogicGate : public Hardwares
{
public:
    Q_OBJECT
    int gateType = -1;  //表示逻辑门的种类
    Voltage pins[LGATE_PIN_NUM];
    Counter<LogicGate> c;

public:
    LogicGate(MicroCom::GateType type, QString gateName=nullptr);
    void setPinVoltage(MicroCom::Pins pin, Voltage value);
    Voltage getPinVoltage(MicroCom::Pins pin);
    void handlePinVolChanges(MicroCom::Pins pin, Voltage value);
    static int howMany(){return Counter<LogicGate>::howMany();}
};

#endif // NAND_H
