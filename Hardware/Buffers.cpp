#include "Buffers.h"

Buffers::Buffers(MicroCom::BufferType type, QString bufferName){
    status=0;
    //初始化命名
    QString temp;
    bftype = type;
    if(bftype==MicroCom::BF_74LS244){
        temp="74LS244_";
    }
    else{
        temp="74LS245_";
    }
    if(bufferName==nullptr){
        bufferName = temp+QString::number(c.howMany());
    }
    setHardwareName(bufferName);
    //初始化引脚
    for(int i=0;i<BUFFER_PIN_NUM;i++){
        pins[i]=inf;
    }
}

void Buffers::setPinVoltage(MicroCom::Pins pin, Voltage value){
    pins[pin]=value;
    emit pinVolChanged(pin);
}

//获取引脚电平函数
Voltage Buffers::getPinVoltage(MicroCom::Pins pin){
    return pins[pin];
}

void Buffers::handlePinVolChanges(MicroCom::Pins pin, Voltage value){
    pins[pin]=value;
    if(bftype == MicroCom::BF_74LS244){
        if(pin==MicroCom::BF4_Ga || pin==MicroCom::BF4_Gb){
            if(status != 1){
                status++;
                return;
            }
            for(int i=0;i<4;i++){
                if(Buffers::getPinVoltage(MicroCom::BF4_Ga)==low){
                    setPinVoltage(static_cast<MicroCom::Pins>(i+4),pins[i]);
                }
                else{
                    setPinVoltage(static_cast<MicroCom::Pins>(i+4),inf);
                }
                if(Buffers::getPinVoltage(MicroCom::BF4_Gb)==low){
                    setPinVoltage(static_cast<MicroCom::Pins>(i+12),pins[i+8]);
                }
                else{
                    setPinVoltage(static_cast<MicroCom::Pins>(i+12),inf);
                }
            }
            //结果
            qDebug()<<"1A1="<<Buffers::getPinVoltage(MicroCom::BF4_1O1);
            qDebug()<<"1A2="<<Buffers::getPinVoltage(MicroCom::BF4_1O2);
            qDebug()<<"1A3="<<Buffers::getPinVoltage(MicroCom::BF4_1O3);
            qDebug()<<"1A4="<<Buffers::getPinVoltage(MicroCom::BF4_1O4);
            qDebug()<<"2A1="<<Buffers::getPinVoltage(MicroCom::BF4_2O1);
            qDebug()<<"2A2="<<Buffers::getPinVoltage(MicroCom::BF4_2O2);
            qDebug()<<"2A3="<<Buffers::getPinVoltage(MicroCom::BF4_2O3);
            qDebug()<<"2A4="<<Buffers::getPinVoltage(MicroCom::BF4_2O4);
        }
        else{
            return;
        }
    }
    else{
        if(pin==MicroCom::BF5_g){
            //如果使能端为低电平（有效）
            if(value==low){
                status=1;
                //如果DIR为高电平，则数据从A(100~107,pins[0~7])->B(108~115,pins[8~15])
                if(Buffers::getPinVoltage(MicroCom::BF5_DIR)==high){
                    for(int i=0;i<8;i++){
                        setPinVoltage(static_cast<MicroCom::Pins>(i+8),pins[i]);
                    }
                    qDebug()<<"========"<<getHardwareName()<<"A->B works========";
                    qDebug()<<"B0="<<Buffers::getPinVoltage(MicroCom::BF5_B0);
                    qDebug()<<"B1="<<Buffers::getPinVoltage(MicroCom::BF5_B1);
                    qDebug()<<"B2="<<Buffers::getPinVoltage(MicroCom::BF5_B2);
                    qDebug()<<"B3="<<Buffers::getPinVoltage(MicroCom::BF5_B3);
                    qDebug()<<"B4="<<Buffers::getPinVoltage(MicroCom::BF5_B4);
                    qDebug()<<"B5="<<Buffers::getPinVoltage(MicroCom::BF5_B5);
                    qDebug()<<"B6="<<Buffers::getPinVoltage(MicroCom::BF5_B6);
                    qDebug()<<"B7="<<Buffers::getPinVoltage(MicroCom::BF5_B7);
                }
                //如果DIR为低电平，则数据从B(108~115,pins[8~15])端口->A(100~107,pins[0~7])端口
                else{
                    for(int i=0;i<8;i++){
                        setPinVoltage(static_cast<MicroCom::Pins>(i),pins[i+8]);
                    }
                    qDebug()<<"========"<<getHardwareName()<<"B->A works========";
                    qDebug()<<"A0="<<Buffers::getPinVoltage(MicroCom::BF5_A0);
                    qDebug()<<"A1="<<Buffers::getPinVoltage(MicroCom::BF5_A1);
                    qDebug()<<"A2="<<Buffers::getPinVoltage(MicroCom::BF5_A2);
                    qDebug()<<"A3="<<Buffers::getPinVoltage(MicroCom::BF5_A3);
                    qDebug()<<"A4="<<Buffers::getPinVoltage(MicroCom::BF5_A4);
                    qDebug()<<"A5="<<Buffers::getPinVoltage(MicroCom::BF5_A5);
                    qDebug()<<"A6="<<Buffers::getPinVoltage(MicroCom::BF5_A6);
                    qDebug()<<"A7="<<Buffers::getPinVoltage(MicroCom::BF5_A7);
                }
            }
            //如果不是低电平（非有效）
            else{
                if(status==0){
                    return;
                }
                status=0;
                for(int i=0;i<16;i++){
                    setPinVoltage(static_cast<MicroCom::Pins>(i),inf);
                }
                qDebug()<<"========"<<getHardwareName()<<"使能端无效========";
                qDebug()<<"=======所有引脚已被置为高阻=======";
            }
        }
    }
    return;
}
