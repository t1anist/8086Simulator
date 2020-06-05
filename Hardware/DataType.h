#ifndef DATATYPE_H
#define DATATYPE_H

#define ADDRNUM 20
#define DATANUM 16

/** 常量 **/
const int CPU_PIN_NUM = 36;
const int CPU_REG_NUM = 14;
//组合逻辑电路
const int DECODER_PIN_NUM = 14;
const int LATCH_PIN_NUM = 18;
const int BUFFER_PIN_NUM = 18;
//可编程芯片
const int PPI_PIN_NUM = 37;
//逻辑门电路
const int LGATE_PIN_NUM = 3;

namespace MicroCom{

//引脚号
enum Pins{
    /** CPU-Pins(1-33) **/
    CP_AD0=0,
    CP_AD1=1,
    CP_AD2=2,
    CP_AD3=3,
    CP_AD4=4,
    CP_AD5=5,
    CP_AD6=6,
    CP_AD7=7,
    CP_AD8=8,
    CP_AD9=9,
    CP_AD10=10,
    CP_AD11=11,
    CP_AD12=12,
    CP_AD13=13,
    CP_AD14=14,
    CP_AD15=15,
    CP_AS16=16,
    CP_AS17=17,
    CP_AS18=18,
    CP_AS19=19,
    CP_rd=20,
    CP_wr=21,
    CP_Mio=22,
    CP_ALE=23,
    CP_DTr=24,
    CP_den=25,
    CP_bhe=26,
    CP_NMI=27,
    CP_INTR=28,
    CP_inta=29,
    CP_READY=30,
    CP_RESET=31,
    CP_test=32,
    CP_CLK=33,

    /** Latch-Pins(0-17) **/
    LA_DI0=0,
    LA_DI1=1,
    LA_DI2=2,
    LA_DI3=3,
    LA_DI4=4,
    LA_DI5=5,
    LA_DI6=6,
    LA_DI7=7,
    LA_DO0=8,
    LA_DO1=9,
    LA_DO2=10,
    LA_DO3=11,
    LA_DO4=12,
    LA_DO5=13,
    LA_DO6=14,
    LA_DO7=15,
    LA_G=16,
    LA_oe=17,

    /** Decoder-Pins(0-13) **/
    DE_G1=0,
    DE_G2a=1,
    DE_G2b=2,
    DE_A=3,
    DE_B=4,
    DE_C=5,
    DE_y0=6,
    DE_y1=7,
    DE_y2=8,
    DE_y3=9,
    DE_y4=10,
    DE_y5=11,
    DE_y6=12,
    DE_y7=13,

    /** Buffer244-Pins(0-17) **/
    BF4_1I1=0,
    BF4_1I2=1,
    BF4_1I3=2,
    BF4_1I4=3,
    BF4_1O1=4,
    BF4_1O2=5,
    BF4_1O3=6,
    BF4_1O4=7,
    BF4_2I1=8,
    BF4_2I2=9,
    BF4_2I3=10,
    BF4_2I4=11,
    BF4_2O1=12,
    BF4_2O2=13,
    BF4_2O3=14,
    BF4_2O4=15,
    BF4_Ga=16,
    BF4_Gb=17,

    /** Buffer245-Pins(0-17) **/
    BF5_A0=0,
    BF5_A1=1,
    BF5_A2=2,
    BF5_A3=3,
    BF5_A4=4,
    BF5_A5=5,
    BF5_A6=6,
    BF5_A7=7,
    BF5_B0=8,
    BF5_B1=9,
    BF5_B2=10,
    BF5_B3=11,
    BF5_B4=12,
    BF5_B5=13,
    BF5_B6=14,
    BF5_B7=15,
    BF5_DIR=16,
    BF5_g=17,

    /** PPI-Pins(0-36) **/
    PP_PA0=0,
    PP_PA1=1,
    PP_PA2=2,
    PP_PA3=3,
    PP_PA4=4,
    PP_PA5=5,
    PP_PA6=6,
    PP_PA7=7,
    PP_PB0=8,
    PP_PB1=9,
    PP_PB2=10,
    PP_PB3=11,
    PP_PB4=12,
    PP_PB5=13,
    PP_PB6=14,
    PP_PB7=15,
    PP_PC0=16,
    PP_PC1=17,
    PP_PC2=18,
    PP_PC3=19,
    PP_PC4=20,
    PP_PC5=21,
    PP_PC6=22,
    PP_PC7=23,
    PP_D0=24,
    PP_D1=25,
    PP_D2=26,
    PP_D3=27,
    PP_D4=28,
    PP_D5=29,
    PP_D6=30,
    PP_D7=31,
    PP_A0=32,
    PP_A1=33,
    PP_wr=34,
    PP_rd=35,
    PP_cs=36,

    /** Logic Gates **/
    LG_I1 = 0,    //输入1
    LG_I2 = 1,    //输入2
    LG_O = 2      //输出

};

enum PPIWorkWay{
    PPI_A0B0C0=0,
    PPI_A1B0C0=1,
    PPI_A2B0C0=2,
    PPI_A0B1C0=3,
    PPI_A1B1C0=4,
    PPI_A2B1C0=5,
};

//寄存器号
enum Regs{
    ax=1,
    bx=2,
    cx=3,
    dx=4,
    cs=5,
    ds=6,
    es=7,
    ss=8,
    ip=9,
    flags=10,
    bp=11,
    sp=12,
    si=13,
    di=14,
    io=15,      //8255A
    ct=16,      //counter 8253/8254
    al=17,
    bl=18,
    cl=19,
    dl=20,
    ah=21,
    bh=22,
    ch=23,
    dh=24,
    no=0
};

enum RegsLen{
    dbyte=16,
    byte=8
};

enum GateType{
    AND=0,
    OR=1,
    NOT=2,
    NAND=3,
    NOR=4,  //或非门
    XOR=5   //异或门
};

enum BufferType{
    BF_74LS244,
    BF_74LS245
};

enum Dir{
    In,
    Out
};
}

//引脚电平
enum Voltage{
    high=5,
    low=0,
    inf=-1
};


struct MemoryUnit{
    MicroCom::Regs based = MicroCom::no;
    MicroCom::Regs indexed = MicroCom::no;
    MicroCom::Regs prefixed = MicroCom::no;
    int addr_offset = -1;
    MemoryUnit(MicroCom::Regs source, int count=0, MicroCom::Regs prefix=MicroCom::no){
        based = source;
        addr_offset = count;
        prefixed = prefix;
    }
    MemoryUnit(MicroCom::Regs base, MicroCom::Regs index, int count=0, MicroCom::Regs prefix=MicroCom::no){
        based = base;
        indexed = index;
        prefixed = prefix;
        addr_offset = count;
    }
    MemoryUnit(int addr){
        addr_offset = addr;
    }
};


//模板类，用于类的计数
template<typename T>
class Counter {
public:
    Counter() { ++count; }
    Counter(const Counter&) { ++count; }
    ~Counter() { --count; }
    static int howMany()
    { return count; }

private:
    static int count;
};

template<typename T>
int
Counter<T>::count = 0; // this now can go in header


#endif // DATATYPE_H
