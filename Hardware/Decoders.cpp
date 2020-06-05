#include "Decoders.h"

Decoders::Decoders(QString decoderName){
    status=8;
    buffer = 0;
    if(decoderName == nullptr){
        decoderName = "74LS138_" + QString::number(c.howMany());
    }
    //init the pins
    setHardwareName(decoderName);
    for(int i=0;i<6;i++){
        pins[i]=inf;
    }
    for(int i=6;i<DECODER_PIN_NUM;i++){
        pins[i]=high;
    }
}

/****************************************************
 - Function：set the pin's voltage
 - Description：
 - Input：the pin's id and voltage value
 - Output：signal of the pin's voltage have changed
*****************************************************/
void Decoders::setPinVoltage(MicroCom::Pins pin, Voltage value){
    pins[pin]=value;
    emit pinVolChanged(pin);
}

/****************************************************
 - Function：get the pin's voltage
 - Description：Overridden virtual function
 - Input：the pin's id(MicroCom::Pins)
 - Return：the pin's voltage(Voltage)
*****************************************************/
Voltage Decoders::getPinVoltage(MicroCom::Pins pin){
    return pins[pin];
}

/***************************************************************
 - Function：deal with the signal of the upriver pin's vol changes
 - Description：Overridden virtual function
 - Input：the pin's id and voltage value
 - Output：output pins' voltage value change
***************************************************************/
void Decoders::handlePinVolChanges(MicroCom::Pins pin, Voltage value){
    pins[pin]=value;
    //首先只有触发使能端才会有效
    if(pin==MicroCom::DE_G1 || pin==MicroCom::DE_G2a || pin==MicroCom::DE_G2b){
        //buffer func: make sure the decoder only executes once in the cycle
        if(buffer!=2){
            buffer++;
            return;
        }
        buffer=0;
        //works only whendx the enable pins' voltage are all low
        if(Decoders::getPinVoltage(MicroCom::DE_G1)==high
                && Decoders::getPinVoltage(MicroCom::DE_G2a)==low
                && Decoders::getPinVoltage(MicroCom::DE_G2b)==low){
            int rst = 0;
            //ABC（0~7）->相应的"y_"变为低电平
            for(int i=0;i<3;i++){
                //C:pins[5],B:pins[4],A:pins[3]
                if(pins[i+3]==high){
                    rst += 1<<i;
                }
            }
            for(int i=0;i<8;i++){
                setPinVoltage(static_cast<MicroCom::Pins>(i+6),high);
            }
            //译码器正在执行译码功能
            setPinVoltage(static_cast<MicroCom::Pins>(rst+6),low);
            status = rst;//status的范围为0~7,表示译码器正在工作
            qDebug()<<"========="<<getHardwareName()<<"WORK========";
            qDebug()<<"y0="<<Decoders::getPinVoltage(MicroCom::DE_y0);
            qDebug()<<"y1="<<Decoders::getPinVoltage(MicroCom::DE_y1);
            qDebug()<<"y2="<<Decoders::getPinVoltage(MicroCom::DE_y2);
            qDebug()<<"y3="<<Decoders::getPinVoltage(MicroCom::DE_y3);
            qDebug()<<"y4="<<Decoders::getPinVoltage(MicroCom::DE_y4);
            qDebug()<<"y5="<<Decoders::getPinVoltage(MicroCom::DE_y5);
            qDebug()<<"y6="<<Decoders::getPinVoltage(MicroCom::DE_y6);
            qDebug()<<"y7="<<Decoders::getPinVoltage(MicroCom::DE_y7);
            qDebug()<<"========="<<getHardwareName()<<"END=========";
        }
        //使能端非有效电平时
        else{
            //如果译码器处于初始化或者未工作状态，则不重复执行还原操作
            if(status==8){
                return;
            }
            qDebug()<<"========="<<getHardwareName()<<"输出端全置1=========";
            setPinVoltage(static_cast<MicroCom::Pins>(status+6),high);
            //还原译码器输出端口，将译码器的工作状态置为未工作。
            status = 8;
        }
    }
}


