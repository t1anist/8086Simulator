#ifndef CPUS_H
#define CPUS_H
#include "Hardwares.h"
#include "DataType.h"
#include <QDebug>
#include <QTimer>

class CPUs : public Hardwares
{
public:
    CPUs(QString cpuName = nullptr);
private:
    Q_OBJECT
    int innerReg[CPU_REG_NUM];
    Voltage pins[CPU_PIN_NUM];
    int clk_cpu;
    int address;
    int T;  //周期
    int data;
    Counter<CPUs> c;
public:
    //set the pin's voltage
    void setPinVoltage(MicroCom::Pins pin, Voltage value);
    void handlePinVolChanges(MicroCom::Pins pin, Voltage value);
    //get the pin's voltage
    Voltage getPinVoltage(MicroCom::Pins pin);

    //set the ADDRESS BUS by addr
    void setAddrDataPinsVoltage(int addr,bool isData=true);
    //get the DATA BUS value
    int getDataValue(bool isDataBus=true);

    //set the Register's value
    void setRegValue(MicroCom::Regs reg, int value);
    void setRegValue(MicroCom::Regs reg, Voltage biValue, int bit);

    //get the Register's value
    int getRegValue(MicroCom::Regs reg);
    int getRegValue(MicroCom::Regs reg, int pos);

    //Bus Timing
    int readBusCycle(int phyAddr, bool isMemory = true);
    void writeBusCycle(int phyAddr, int value,  bool isMemory = true);

    //判断是否为奇数
    bool isOdd(int i);
    void resetCPU();
    static int howMany(){return Counter<CPUs>::howMany();}

signals:
    void resetSignal();
};

#endif // CPUS_H
