#ifndef BUFFERS_H
#define BUFFERS_H
#include "Hardwares.h"


class Buffers : public Hardwares
{
private:
    Q_OBJECT
    Voltage pins[BUFFER_PIN_NUM];
    Counter<Buffers> c;
    MicroCom::BufferType bftype;
    int status;

public:
    Buffers(MicroCom::BufferType type, QString bufferName=nullptr);
    void setPinVoltage(MicroCom::Pins pin, Voltage value);
    void handlePinVolChanges(MicroCom::Pins pin, Voltage);
    Voltage getPinVoltage(MicroCom::Pins pin);
    //计数器
    static int howMany(){return Counter<Buffers>::howMany();}
};

#endif // BUFFERS_H
