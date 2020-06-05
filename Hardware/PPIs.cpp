#include "PPIs.h"

PPIs::PPIs(QString ppiName){
    controlReg = 0;
    if(ppiName==nullptr){
        ppiName = "8255A_" + QString::number(c.howMany());
    }
    setHardwareName(ppiName);
    for(int i=0;i<PPI_PIN_NUM;i++){
        pins[i]=inf;
    }
}

void PPIs::setPinVoltage(MicroCom::Pins pin, Voltage value){
    pins[pin]=value;
    emit pinVolChanged(pin);
}

Voltage PPIs::getPinVoltage(MicroCom::Pins pin){
    return pins[pin];
}

int PPIs::getDataValue(){
    int data = 0;
    qDebug()<<"=============="<<getHardwareName()<<"START GET DATA===============";
    for(int i=0;i<8;i++){
        if(pins[i+24]==high){
            data += 1<<i;
        }
    }
    qDebug()<<"DATA IS"<<data;
    qDebug()<<"=============="<<getHardwareName()<<"END GET DATA===============";
    return data;
}

//设置ABC和数据端口的引脚电平
void PPIs::setPortPinVoltage(bool isRead, int offset, int len){
    int outPort = 0;
    int inPort = 0;
    if(isRead){
        inPort = offset;
        outPort = 24;
    }
    else{
        inPort = 24;
        outPort = offset;
    }
    for(int i=0;i<len;i++){
        setPinVoltage(static_cast<MicroCom::Pins>(i+outPort),pins[i+inPort]);
    }
}

//要给方式1、方式2留出实现的接口（√）
void PPIs::handlePinVolChanges(MicroCom::Pins pin, Voltage value){
    pins[pin]=value;
    //如果要进行读写
    if( pin==MicroCom::PP_rd || pin==MicroCom::PP_wr ){
        //如果使能端有效
        if(PPIs::getPinVoltage(MicroCom::PP_cs)==low){
            int port = 0;
            Voltage A0 = PPIs::getPinVoltage(MicroCom::PP_A0);
            Voltage A1 = PPIs::getPinVoltage(MicroCom::PP_A1);
            if(A0==high)  port += 1;
            if(A1==high)  port += 2;
            if(port == 3){  //读写控制字寄存器
                if(PPIs::getPinVoltage(MicroCom::PP_rd)==low){      //如果读寄存器
                    int pos =1;
                    for(int i=0;i<8;i++){
                        if((controlReg & pos)>0){
                            setPinVoltage(static_cast<MicroCom::Pins>(i+24),high);
                        }
                        else{
                            setPinVoltage(static_cast<MicroCom::Pins>(i+24),low);
                        }
                        pos <<= 1;
                    }
                }           //如果写寄存器
                else if(PPIs::getPinVoltage(MicroCom::PP_wr)==low){
                    setControlRegValue(getDataValue());
                    if(getControlRegValue(7)==0){
                        Voltage value = inf;
                        int pos = 0;
                        if(getControlRegValue(0)==1){
                            value = high;
                        }
                        else{
                            value = low;
                        }
                        for(int i=0;i<3;i++){
                            if(getControlRegValue(i+1)==1){
                                pos += 1<<i;
                            }
                        }
                        setPinVoltage(static_cast<MicroCom::Pins>(pos+16),value);
                    }
                }
                else{
                    return;
                }
            }
            else{   //读写ABC端口
                switch (getWorkWay()) {
                case MicroCom::PPI_A2B1C0:
                    //A(方式2) B(方式1) C(方式0)
                    break;
                case MicroCom::PPI_A2B0C0:
                    //A(方式2) B(方式0) C(方式0)
                    break;
                case MicroCom::PPI_A1B1C0:
                    //A(方式1) B(方式1) C(方式0)
                    break;
                case MicroCom::PPI_A1B0C0:
                    //A(方式1) B(方式0) C(方式0)
                    break;
                case MicroCom::PPI_A0B1C0:
                    //A(方式0) B(方式1) C(方式0)
                    break;
                default:
                    if(PPIs::getPinVoltage(MicroCom::PP_wr)==low){
                        handleIoMode(false, port);
                    }
                    else if(PPIs::getPinVoltage(MicroCom::PP_rd)==low){
                        handleIoMode(true, port);
                    }
                    break;
                }
            }
        }
    }
    return;
}

//IOmode
void PPIs::handleIoMode(bool isRead, int port){
    QString mode = "";
    int modeFlag = 1;
    if(isRead){
        mode = "READ";
        modeFlag = 1;
    }
    else{
        mode = "WRITE";
        modeFlag = 0;
    }
    qDebug()<<"=========="<<getHardwareName()<<"START =============";
    qDebug()<<"================"<<mode<<"MODE ================";
    switch(port){
    case 0: //port A
        qDebug()<<"======= START"<<mode<<"PORT A =========";
        if(getControlRegValue(4)==modeFlag){
            setPortPinVoltage(isRead,0);
        }
        else{
            qDebug()<<"ERROR: PORT A IN WRONG WORK WAY";
        }
        break;
    case 1: //port B
        qDebug()<<"======= START"<<mode<<"PORT B =========";
        if(getControlRegValue(1)==modeFlag){
            setPortPinVoltage(isRead,8);
        }
        else{
            qDebug()<<"ERROR: PORT B IN WRONG WORK WAY";
        }
        break;
    default: //port C
        if(getControlRegValue(0)==modeFlag){
            qDebug()<<"======= START"<<mode<<"PORT C_LOW =========";
            setPortPinVoltage(isRead,16,4);
            qDebug()<<"======="<<mode<<"PORT C_LOW SUCCESS =========";
        }
        else{
            qDebug()<<"ERROR: PORT C_LOW IN WRONG WORK WAY";
        }
        if(getControlRegValue(3)==modeFlag){
            qDebug()<<"======= START"<<mode<<"PORT C_HIGH =========";
            setPortPinVoltage(isRead,20,4);
            qDebug()<<"======="<<mode<<"PORT C_HIGH SUCCESS =========";
        }
        else{
            qDebug()<<"ERROR: PORT C_HIGH IN WRONG WORK WAY";
        }
        break;
    }
    return;
}


//set the Register's value
void PPIs::setControlRegValue(int value){
    controlReg &= 0;
    controlReg |= value;
}

//set the Register's value by pos
void PPIs::setControlRegValue(Voltage biValue, int pos){
    unsigned short flag = 0;
    unsigned short temp = 1;
    if(biValue==high){
        flag = 1;
    }
    flag <<= pos;
    temp <<= pos;
    temp = ~temp;
    controlReg &= temp;
    controlReg |= flag;
}


//获取控制字寄存器的值
int PPIs::getControlRegValue(){
    return controlReg;
}


//获取控制字寄存器某位的值
int PPIs::getControlRegValue(int pos){
    if(pos<0||pos>7){
        qDebug()<<"pos wrong!";
        return -1;
    }
    unsigned short flag=1;
    flag <<= pos;
    if(( pos&controlReg )>0){
        return 1;
    }
    else{
        return 0;
    }
}

//获取ABC三个端口的方式
MicroCom::PPIWorkWay PPIs::getWorkWay(){
    unsigned short check = controlReg & 0x64;
    switch(check){
    case 0:
        return MicroCom::PPI_A0B0C0;
    case 4:
        return MicroCom::PPI_A0B1C0;
    case 32:
        return MicroCom::PPI_A1B0C0;
    case 36:
        return MicroCom::PPI_A1B1C0;
    case 64:
    case 96:
        return MicroCom::PPI_A2B0C0;
    default:
        return MicroCom::PPI_A2B1C0;
    }
}

