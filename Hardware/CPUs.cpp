#include "CPUs.h"

CPUs::CPUs(QString cpuName){
    T=50;
    if(cpuName == nullptr){
        cpuName = "CPU_"+QString::number(c.howMany());
    }
    setHardwareName(cpuName);
    //init pin's voltage
    for(int i=0;i<CPU_PIN_NUM;i++){
        pins[i]=inf;
    }
}

/****************************************************
 - Function：get the register's value
 - Input：the reg's id
 - Return：register's value(int)
*****************************************************/
int CPUs::getRegValue(MicroCom::Regs reg){
    if( reg >= MicroCom::ax && reg < MicroCom::al ){
        return innerReg[reg];
    }
    else{
        if(reg >=MicroCom::al && reg<=MicroCom::dl){
            return (innerReg[reg-16] & 0x00ff);
        }
        else{
            return (innerReg[reg-20] >> 8);
        }
    }
}


/****************************************************
 - Function：get the data from DATA BUS
 - Input：
 - Return：data value(unsigned short)
*****************************************************/
int CPUs::getDataValue(bool isDataBus){
    if(isDataBus){
        int rst=0;
        int len = 8;
        qDebug()<<"===========8086CPU START GET DATA============";
        if(pins[MicroCom::CP_bhe]==low){
            len = 16;
        }
        for(int i=0;i<len;i++){
            if(pins[i]==high){
                rst += 1<<i;
            }
        }
        qDebug()<<"===========DATA IS"<<rst<<"==============";
        return rst;
    }
    else{
        return data;
    }

}

/****************************************************
 - Function：get the reg's bit value
 - Description：bit limit(16-bit:0-15 8-bit:0-7)
 - Input：the reg's id(MicroCom::Pins)
 - Return：the reg's bit value
*****************************************************/
int CPUs::getRegValue(MicroCom::Regs reg, int bit){
    int flag = 1;
    flag <<= bit;
    int rst = getRegValue(reg);
    if((rst & flag)>0){
        return 1;
    }
    else{
        return 0;
    }
}

/****************************************************
 - Function：set the register's value
 - Description：the value is a signed number
 - Input：the reg's id(MicroCom::Regs) and value(int)
 - Return：
*****************************************************/
void CPUs::setRegValue(MicroCom::Regs reg, int value){
    int offset = 16;
    if(reg>=MicroCom::ax && reg<=MicroCom::di){
        innerReg[reg] = value;
    }
    else{
        value &= 0x00ff;
        if( reg>=MicroCom::al && reg<=MicroCom::dl){
            innerReg[reg-offset] &= 0xff00;
        }
        else{
            offset = 20;
            value <<= 8;
            innerReg[reg-offset] &= 0x00ff;
        }
        innerReg[reg-offset] |= value;
    }
}

/****************************************************
 - Function：set register's particular bit value
 - Description：bit limit(16-bit:0-15 8-bit:0-7)
 - Input：the reg's id(MicroCom::Regs)
 - Return：
*****************************************************/
void CPUs::setRegValue(MicroCom::Regs reg, Voltage biValue, int bit){
    unsigned short flag = 0;
    unsigned short temp = 1;
    if(biValue==high){
        flag = 1;
    }
    //Registers excluding ah, bh, ch and dh
    if(reg>=MicroCom::ax && reg<MicroCom::ah){
        flag <<= bit;
        temp <<= bit;
    }
    //ah, bh, ch or dh
    else{
        flag <<= (bit+8);
        temp <<= (bit+8);
    }
    temp = ~temp;
    innerReg[reg] &= temp;
    innerReg[reg] |= flag;
}

/****************************************************
 - Function：set ADDR_DATA BUS pins' voltage
 - Description：
 - Input：address value(int) and ADDR-or-DATA mode(bool)
 - Return：
*****************************************************/
void CPUs::setAddrDataPinsVoltage(int value, bool isData){
    int pos = 1;
    int len = ADDRNUM;
    if(isData){
        len = DATANUM;
    }
    for(int i=0;i<len;i++){
        if((value & pos)>0){
            setPinVoltage(static_cast<MicroCom::Pins>(i),high);
        }
        else{
            setPinVoltage(static_cast<MicroCom::Pins>(i),low);
        }
        pos <<= 1;
    }
    delaymsec(T);
}


/****************************************************
 - Function：carry out READ Bus Timing
 - Description：
 - Input：address value(int) and Memory-or-IO mode(bool)
 - Return：Memory or IO interface's data value
*****************************************************/
int CPUs::readBusCycle(int phyAddr, bool isMemory){
    address = phyAddr;
    qDebug()<<"========READ BUS CYCLE===========";
    /** ==============T1 START================**/
    //  1.Mio 读存储器则为高电平，读IO则为低电平（整个周期有效）
    //  2.AD1~AD16、AS17~AS20 输出地址
    //  3.bhe 输出低电平表示有效
    //  4.ALE 是一个正脉冲（地址锁存信号有效）
    //  5.DTr 为低电平，（用来改变双向缓冲器的数据传输方向，改变缓冲器的DIR）
    //  6.延时半周期后，ALE 变为低电平
    qDebug()<<"============T1 start=============";
    qDebug()<<"now start to change address lins";
    if(isMemory){
        setPinVoltage(MicroCom::CP_Mio,high);
    }
    else{
        setPinVoltage(MicroCom::CP_Mio,low);
    }
    setAddrDataPinsVoltage(address,false);
    setPinVoltage(MicroCom::CP_bhe,low);
    setPinVoltage(MicroCom::CP_ALE,high);
    setPinVoltage(MicroCom::CP_DTr,low);
    delaymsec(T);
    qDebug()<<"============T1 HALF=============";
    setPinVoltage(MicroCom::CP_ALE,low);
    qDebug()<<"============T1 END =============";

    /** ==============T2 START================**/
    //  1.AD1~AD16 变为高阻态（无意义）
    //  2.rd  在后半周期变为低电平，表示允许读取（rd变低意味着数据到达数据线上）
    //  3.den 在后半周期变为低电平，表示数据有效，用来实现缓冲器的选通（den变低意味着数据从缓冲器到达CPU数据线）
    //  4.bhe 变为高电平

    delaymsec(2*T);
    qDebug()<<"============T2 START=============";
    setPinVoltage(MicroCom::CP_rd,low);
    setPinVoltage(MicroCom::CP_bhe,high);
    delaymsec(T);
    qDebug()<<"============T2 HALF=============";
    setPinVoltage(MicroCom::CP_den,low);
    qDebug()<<"============T2 END=============";

    /** ==============T3 START================**/
    //  1.AD1~AD16 接受数据（从数据线上接收数据）
    delaymsec(2*T);
    qDebug()<<"============T3 START============";
    data = getDataValue();
    qDebug()<<"============T3 END=============";

    /** ==============T4 START================**/
    //  1.AD1~AD16 变为高阻态
    //  2.den 变为高电平
    //  3.rd  变为高电平
    delaymsec(2*T);
    qDebug()<<"============T4 START============";
    for(int i=0;i<ADDRNUM;i++){
        setPinVoltage(static_cast<MicroCom::Pins>(i),inf);
    }
    setPinVoltage(MicroCom::CP_rd,high);
    setPinVoltage(MicroCom::CP_den,high);
    setPinVoltage(MicroCom::CP_Mio,inf);
    qDebug()<<"============T4 END=============";
    return data;
}


/*********************************************************
 - Function：Carry out the WRITE Bus Timing
 - Description：
 - Input：address value, data value and Memory-or-IO mode
 - Output：data into DATA BUS, and then into Memory or IO
*********************************************************/

void CPUs::writeBusCycle(int phyAddr, int value, bool isMemory){
    address = phyAddr;
    qDebug()<<"========WRITE BUS CYCLE===========";
    /** ==============T1 START================**/
    //  1.Mio 读存储器则为高电平，读IO则为低电平（整个周期有效）
    //  2.AD1~AD16、AS17~AS20 输出地址
    //  3.bhe 输出低电平表示高8位有效
    //  4.ALE 是一个正脉冲（地址锁存信号有效）
    //  5.DTr 为高电平，（用来改变双向缓冲器的数据传输方向，改变缓冲器的DIR）
    //  6.延时半周期后，ALE 变为低电平
    qDebug()<<"============T1 start=============";
    qDebug()<<"now start to change address lins";
    if(isMemory){
        setPinVoltage(MicroCom::CP_Mio,high);
    }
    else{
        setPinVoltage(MicroCom::CP_Mio,low);
    }
    setAddrDataPinsVoltage(address,false);
    setPinVoltage(MicroCom::CP_bhe,low);
    setPinVoltage(MicroCom::CP_ALE,high);
    setPinVoltage(MicroCom::CP_DTr,high);
    delaymsec(T);
    qDebug()<<"============T1 HALF=============";
    setPinVoltage(MicroCom::CP_ALE,low);
    qDebug()<<"============T1 END =============";

    /** ==============T2 START================**/
    //  1.AD1~AD16 输出数据
    //  2.den 变为低电平，表示数据有效，用来实现缓冲器的选通（den变低意味着数据从CPU到达缓冲器）
    //  3.wr  在后半周期变为低电平，表示允许写入（rd变低意味着数据从缓冲器到达存储器或IO端口）
    //  4.bhe 变为高电平
    delaymsec(2*T);
    setAddrDataPinsVoltage(value);
    qDebug()<<"============T2 START=============";
    setPinVoltage(MicroCom::CP_den,low);
    setPinVoltage(MicroCom::CP_bhe,high);
    delaymsec(T);
    qDebug()<<"============T2 HALF=============";
    setPinVoltage(MicroCom::CP_wr,low); //触发8255A、存储器工作
    qDebug()<<"============T2 END=============";

    /** ==============T3 START================**/
    delaymsec(2*T);
    qDebug()<<"============T3 START============";
    qDebug()<<"============T3 END=============";

    /** ==============T4 START================**/
    //  1.AD1~AD16 变为高阻态
    //  2.den 变为高电平
    //  3.wr  变为高电平
    delaymsec(2*T);
    qDebug()<<"============T4 START============";
    for(int i=0;i<ADDRNUM;i++){
        setPinVoltage(static_cast<MicroCom::Pins>(i),inf);
    }
    setPinVoltage(MicroCom::CP_wr,high);
    setPinVoltage(MicroCom::CP_den,high);
    setPinVoltage(MicroCom::CP_Mio,inf);
    qDebug()<<"============T4 END=============";
    return;
}

/****************************************************
 - Function：set the pin's voltage
 - Description：
 - Input：the pin's id and voltage
 - Output：signal of the pin's voltage have changed
*****************************************************/
void CPUs::setPinVoltage(MicroCom::Pins pin, Voltage value){
    pins[pin]=value;
    emit pinVolChanged(pin);
}

/****************************************************
 - Function：
 - Description：
 - Input：
 - Return：
*****************************************************/
void CPUs::handlePinVolChanges(MicroCom::Pins pin, Voltage value){
    pins[pin]=value;
    if(pin==MicroCom::CP_RESET && value==high){
        resetCPU();
    }
}

/****************************************************
 - Function：get the pin's voltage
 - Description：Overridden virtual function
 - Input：the pin's id(MicroCom::Pins)
 - Return：the pin's voltage(Voltage)
*****************************************************/
Voltage CPUs::getPinVoltage(MicroCom::Pins pin){
    return pins[pin];
}

/************************************************************
 - Function：Reset the 8086CPU
 - Description：Triggered if CP_RESET's voltage is high
 - Input：
 - Output：set pins' voltage and regs' value, and send signal
*************************************************************/
void CPUs::resetCPU(){
    qDebug()<<"====================";
    qDebug()<<"====RESET START!====";
    qDebug()<<"====================";
    /** set registers' value **/
    innerReg[MicroCom::flags]=0;
    innerReg[MicroCom::cs]=0;
    innerReg[MicroCom::ip]=0;
    innerReg[MicroCom::ds]=0;
    innerReg[MicroCom::es]=0;
    innerReg[MicroCom::ss]=0;
    /** set pins' value **/
    for(int i=0;i<ADDRNUM;i++){
        pins[i]=inf;
    }
    setPinVoltage(MicroCom::CP_ALE,low);
    //inta 高电平后呈高阻
    setPinVoltage(MicroCom::CP_inta,high);
    delaymsec(T);
    setPinVoltage(MicroCom::CP_inta,inf);
    //rd 高电平后呈高阻
    setPinVoltage(MicroCom::CP_rd,high);
    delaymsec(T);
    setPinVoltage(MicroCom::CP_rd,inf);
    //wr 高电平后呈高阻
    setPinVoltage(MicroCom::CP_wr,high);
    delaymsec(T);
    setPinVoltage(MicroCom::CP_wr,inf);
    //Mio 高电平后呈高阻
    setPinVoltage(MicroCom::CP_Mio,high);
    delaymsec(T);
    setPinVoltage(MicroCom::CP_Mio,inf);
    //DTr 高电平后呈高阻
    setPinVoltage(MicroCom::CP_DTr,high);
    delaymsec(T);
    setPinVoltage(MicroCom::CP_DTr,inf);
    //den 高电平后呈高阻
    setPinVoltage(MicroCom::CP_den,high);
    delaymsec(T);
    setPinVoltage(MicroCom::CP_den,inf);
    //RESET引脚复位
    pins[MicroCom::CP_RESET]=low;
    qDebug()<<"====================";
    qDebug()<<"===RESET SUCCEES!===";
    qDebug()<<"====================";
    return;
}


/****************************************************
 - Function：Judge whether the number is odd
 - Description：
 - Input：number(int)
 - Return：true or false
*****************************************************/
bool CPUs::isOdd(int num){
    return (num&1) == 1;
}


