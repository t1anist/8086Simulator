#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cp = new CPUs("8086CPU");
    //三个锁存器
    la1 = new Latchs("74LS373_1");
    la2 = new Latchs("74LS373_2");
    la3 = new Latchs("74LS373_3");
    //两个缓冲器
    bf1 = new Buffers(MicroCom::BF_74LS245,"74LS245_1");
    bf2 = new Buffers(MicroCom::BF_74LS245,"74LS245_2");
    //8255A可编程并行IO接口
    pp1 = new PPIs("8255A");
    //74LS138译码器
    de1 = new Decoders("74LS138");
    //与非门
    nand = new LogicGate(MicroCom::NOR,"NAND Gate");

    /** 将CPU的地址线与74LS373锁存器的输入端相连 **/

    //将A0~A7与锁存器la1的DI0~DI7相连
    link(cp,MicroCom::CP_AD0,la1,MicroCom::LA_DI0);
    link(cp,MicroCom::CP_AD1,la1,MicroCom::LA_DI1);
    link(cp,MicroCom::CP_AD2,la1,MicroCom::LA_DI2);
    link(cp,MicroCom::CP_AD3,la1,MicroCom::LA_DI3);
    link(cp,MicroCom::CP_AD4,la1,MicroCom::LA_DI4);
    link(cp,MicroCom::CP_AD5,la1,MicroCom::LA_DI5);
    link(cp,MicroCom::CP_AD6,la1,MicroCom::LA_DI6);
    link(cp,MicroCom::CP_AD7,la1,MicroCom::LA_DI7);
    link(cp,MicroCom::CP_ALE,la1,MicroCom::LA_G);
    //将A8~A15与锁存器la2的DI0~DI7相连
    link(cp,MicroCom::CP_AD8,la2,MicroCom::LA_DI0);
    link(cp,MicroCom::CP_AD9,la2,MicroCom::LA_DI1);
    link(cp,MicroCom::CP_AD10,la2,MicroCom::LA_DI2);
    link(cp,MicroCom::CP_AD11,la2,MicroCom::LA_DI3);
    link(cp,MicroCom::CP_AD12,la2,MicroCom::LA_DI4);
    link(cp,MicroCom::CP_AD13,la2,MicroCom::LA_DI5);
    link(cp,MicroCom::CP_AD14,la2,MicroCom::LA_DI6);
    link(cp,MicroCom::CP_AD15,la2,MicroCom::LA_DI7);
    link(cp,MicroCom::CP_ALE,la2,MicroCom::LA_G);
    //将A16~A20与锁存器la3的DI0~DI3相连
    link(cp,MicroCom::CP_AS16,la3,MicroCom::LA_DI0);
    link(cp,MicroCom::CP_AS17,la3,MicroCom::LA_DI1);
    link(cp,MicroCom::CP_AS18,la3,MicroCom::LA_DI2);
    link(cp,MicroCom::CP_AS19,la3,MicroCom::LA_DI3);
    link(cp,MicroCom::CP_bhe,la3,MicroCom::LA_DI4);
    link(cp,MicroCom::CP_ALE,la3,MicroCom::LA_G);

    /** 将CPU与缓存器相连 **/
    //将D0~D7与缓冲器bf1的A0~A7相连
    link(cp,MicroCom::CP_AD0,bf1,MicroCom::BF5_A0);
    link(cp,MicroCom::CP_AD1,bf1,MicroCom::BF5_A1);
    link(cp,MicroCom::CP_AD2,bf1,MicroCom::BF5_A2);
    link(cp,MicroCom::CP_AD3,bf1,MicroCom::BF5_A3);
    link(cp,MicroCom::CP_AD4,bf1,MicroCom::BF5_A4);
    link(cp,MicroCom::CP_AD5,bf1,MicroCom::BF5_A5);
    link(cp,MicroCom::CP_AD6,bf1,MicroCom::BF5_A6);
    link(cp,MicroCom::CP_AD7,bf1,MicroCom::BF5_A7);
    link(cp,MicroCom::CP_den,bf1,MicroCom::BF5_g);
    link(cp,MicroCom::CP_DTr,bf1,MicroCom::BF5_DIR);
    //将D8~D15与缓冲器bf2的A0~A7相连
    link(cp,MicroCom::CP_AD8,bf2,MicroCom::BF5_A0);
    link(cp,MicroCom::CP_AD9,bf2,MicroCom::BF5_A1);
    link(cp,MicroCom::CP_AD10,bf2,MicroCom::BF5_A2);
    link(cp,MicroCom::CP_AD11,bf2,MicroCom::BF5_A3);
    link(cp,MicroCom::CP_AD12,bf2,MicroCom::BF5_A4);
    link(cp,MicroCom::CP_AD13,bf2,MicroCom::BF5_A5);
    link(cp,MicroCom::CP_AD14,bf2,MicroCom::BF5_A6);
    link(cp,MicroCom::CP_AD15,bf2,MicroCom::BF5_A7);
    link(cp,MicroCom::CP_den,bf2,MicroCom::BF5_g);
    link(cp,MicroCom::CP_DTr,bf2,MicroCom::BF5_DIR);

    /** 将锁存器与译码器74LS138相连 **/
    link(la1,MicroCom::LA_DO0,de1,MicroCom::DE_A);
    link(la1,MicroCom::LA_DO1,pp1,MicroCom::PP_A0);
    link(la1,MicroCom::LA_DO2,pp1,MicroCom::PP_A1);
    link(la1,MicroCom::LA_DO3,de1,MicroCom::DE_G2b);
    link(la1,MicroCom::LA_DO4,de1,MicroCom::DE_B);
    link(la1,MicroCom::LA_DO5,de1,MicroCom::DE_C);
    link(la1,MicroCom::LA_DO6,de1,MicroCom::DE_G1);
    link(la1,MicroCom::LA_DO7,nand,MicroCom::LG_I1);
    link(cp,MicroCom::CP_Mio,nand,MicroCom::LG_I2);
    link(nand,MicroCom::LG_O,de1,MicroCom::DE_G2a);

    /** 将译码器与8255A相连 **/
    link(de1,MicroCom::DE_y6,pp1,MicroCom::PP_cs);
    link(cp,MicroCom::CP_wr,pp1,MicroCom::PP_wr);
    link(cp,MicroCom::CP_rd,pp1,MicroCom::PP_rd);

    /** 将缓冲器与8255A相连 **/
    //数据输出
    link(bf1,MicroCom::BF5_B0,pp1,MicroCom::PP_D0);
    link(bf1,MicroCom::BF5_B1,pp1,MicroCom::PP_D1);
    link(bf1,MicroCom::BF5_B2,pp1,MicroCom::PP_D2);
    link(bf1,MicroCom::BF5_B3,pp1,MicroCom::PP_D3);
    link(bf1,MicroCom::BF5_B4,pp1,MicroCom::PP_D4);
    link(bf1,MicroCom::BF5_B5,pp1,MicroCom::PP_D5);
    link(bf1,MicroCom::BF5_B6,pp1,MicroCom::PP_D6);
    link(bf1,MicroCom::BF5_B7,pp1,MicroCom::PP_D7);
    //数据输入
    link(pp1,MicroCom::PP_D0,bf1,MicroCom::BF5_B0);
    link(pp1,MicroCom::PP_D1,bf1,MicroCom::BF5_B1);
    link(pp1,MicroCom::PP_D2,bf1,MicroCom::BF5_B2);
    link(pp1,MicroCom::PP_D3,bf1,MicroCom::BF5_B3);
    link(pp1,MicroCom::PP_D4,bf1,MicroCom::BF5_B4);
    link(pp1,MicroCom::PP_D5,bf1,MicroCom::BF5_B5);
    link(pp1,MicroCom::PP_D6,bf1,MicroCom::BF5_B6);
    link(pp1,MicroCom::PP_D7,bf1,MicroCom::BF5_B7);

    /** 将缓冲器与8086CPU相连 **/
    link(bf1,MicroCom::BF5_A0,cp,MicroCom::CP_AD0);
    link(bf1,MicroCom::BF5_A1,cp,MicroCom::CP_AD1);
    link(bf1,MicroCom::BF5_A2,cp,MicroCom::CP_AD2);
    link(bf1,MicroCom::BF5_A3,cp,MicroCom::CP_AD3);
    link(bf1,MicroCom::BF5_A4,cp,MicroCom::CP_AD4);
    link(bf1,MicroCom::BF5_A5,cp,MicroCom::CP_AD5);
    link(bf1,MicroCom::BF5_A6,cp,MicroCom::CP_AD6);
    link(bf1,MicroCom::BF5_A7,cp,MicroCom::CP_AD7);
/*
    cp->setRegValue(MicroCom::bx,0);
    cp->setRegValue(MicroCom::cx,0);
    cp->setRegValue(MicroCom::ax,56);
    qDebug()<<"cp.ax="<<cp->getRegValue(MicroCom::ax);
    cp->setRegValue(MicroCom::bh,127);
    qDebug()<<"cp.bx="<<cp->getRegValue(MicroCom::bx);
    cp->setRegValue(MicroCom::cl,127);
    qDebug()<<"cp.cx="<<cp->getRegValue(MicroCom::cx);*/

    //cp->setRegValue(MicroCom::al,0xff);
    mov(MicroCom::ax,0xff);
    out(MicroCom::ax,0x76);
//    add(MicroCom::In,MicroCom::ax,{MicroCom::bx},false);
    //mov(MicroCom::In,MicroCom::ax,MicroCom::bx,0);  //MOV AX,[BX]
    //mov(MicroCom::Out,MicroCom::ax,MicroCom::bx,0); //MOV [BX],AX
    //qDebug()<<pp1->getControlRegValue();
    //cp->writeBusCycle(0x0076,0x93);
    //qDebug()<<pp1->getControlRegValue();
    //cp->readBusCycle(0x0076);
    //cp->readBusCycle(0x0070);
    //cp->readBusCycle(0x0074);

}

MainWindow::~MainWindow()
{
    delete ui;
    ui=nullptr;

    delete cp;
    cp=nullptr;

    delete la1;
    la1=nullptr;

    delete la2;
    la2=nullptr;

    delete la3;
    la3=nullptr;

    delete bf1;
    bf1=nullptr;

    delete bf2;
    bf2=nullptr;

    delete pp1;
    pp1=nullptr;

    delete de1;
    de1=nullptr;

    delete nand;
    nand=nullptr;
}

/** mov指令 **/
//mov 立即数寻址（将直接寻址与立即数寻址合并，通过bool isAddressing区分）
void MainWindow::mov(MicroCom::Regs target, int value){
    cp->setRegValue(target,value);
    return;
}

//
void MainWindow::mov(MemoryUnit m, int imValue){
    int phyAddr = getPhyAddr(m);
    cp->writeBusCycle(phyAddr,imValue);
    return;
}

//mov 寄存器→寄存器
void MainWindow::mov(MicroCom::Regs target, MicroCom::Regs source){
    int value = cp->getRegValue(source);
    cp->setRegValue(target,value);
    return;
}

void MainWindow::mov(MicroCom::Dir dir, MicroCom::Regs reg, MemoryUnit m){
    int phyAddr = getPhyAddr(m);
    int value = 0;
    if(dir == MicroCom::In){
        value = cp->readBusCycle(phyAddr);
        cp->setRegValue(reg,value);
    }
    else{
        value = cp->getRegValue(reg);
        cp->writeBusCycle(phyAddr,value);
    }
    return;
}


/** 出入栈指令 **/
//push
//源操作数为寄存器
void MainWindow::push(MicroCom::Regs source){
    int value = cp->getRegValue(source);
    int newSpValue = cp->getRegValue(MicroCom::sp)-2;
    int stackAddr = cp->getRegValue(MicroCom::ss)*16 + newSpValue;
    cp->writeBusCycle(stackAddr,value);
    cp->setRegValue(MicroCom::sp,newSpValue);
    return;
}

//源操作数为存储单元
void MainWindow::push(MemoryUnit m){
    int phyAddr = getPhyAddr(m);
    int value = cp->readBusCycle(phyAddr);
    int newSpValue = cp->getRegValue(MicroCom::sp)-2;
    int stackAddr = cp->getRegValue(MicroCom::ss)*16 + newSpValue;
    cp->setRegValue(MicroCom::sp,newSpValue);
    cp->writeBusCycle(stackAddr,value);
}

//目的操作数为寄存器
void MainWindow::pop(MicroCom::Regs target){
    int stackAddr = getPhyAddr(MemoryUnit(MicroCom::sp));
    int value = cp->readBusCycle(stackAddr);
    cp->setRegValue(target,value);
    cp->setRegValue(MicroCom::sp,cp->getRegValue(MicroCom::sp)+2);
    return;
}

//目的操作数为存储单元
void MainWindow::pop(MemoryUnit m){
    int stackAddr = getPhyAddr(MemoryUnit(MicroCom::sp));
    int value = cp->readBusCycle(stackAddr);
    int tPhyAddr = getPhyAddr(m);
    cp->writeBusCycle(tPhyAddr,value);
    cp->setRegValue(MicroCom::sp,cp->getRegValue(MicroCom::sp)-2);
    return;
}

/** 获取物理地址函数 **/
int MainWindow::getPhyAddr(MemoryUnit m){
    if(m.based==MicroCom::no){
        return m.addr_offset;
    }
    int phyAddr = 0;
    int segAddr = 0;
    if(m.prefixed != MicroCom::no){
        segAddr = cp->getRegValue(m.prefixed);
    }
    else{
        if(m.based == MicroCom::bp || m.based == MicroCom::sp){
            segAddr = cp->getRegValue(MicroCom::ss);
        }
        else{
            segAddr = cp->getRegValue(MicroCom::ds);
        }
    }
    phyAddr = segAddr*16 + cp->getRegValue(m.based) + m.addr_offset;
    if(m.indexed != MicroCom::no){
        phyAddr += cp->getRegValue(m.indexed);
    }
    return phyAddr;
}


void MainWindow::add(MicroCom::Dir dir, MicroCom::Regs reg, MemoryUnit m, bool isCarry){
    int phyAddr = getPhyAddr(m);
    int mryValue = cp->readBusCycle(phyAddr);
    int regValue = cp->getRegValue(reg);
    if(isCarry){
        regValue += cp->getRegValue(MicroCom::flags,0);
    }
    if(dir == MicroCom::In){
        cp->setRegValue(reg,regValue+mryValue);
    }
    else{
        cp->writeBusCycle(phyAddr,regValue+mryValue);
    }
    return;
}


//连线函数
void MainWindow::link(Hardwares* sender, MicroCom::Pins pinS, Hardwares* receiver, MicroCom::Pins pinR){
    connect(sender, &Hardwares::pinVolChanged, [=](MicroCom::Pins pinC){
        if(pinC==pinS){
            receiver->handlePinVolChanges(pinR,sender->getPinVoltage(pinS));
        }
    });
    return;
}

//输入指令IN
void MainWindow::in(MicroCom::Regs reg, int addr){
    int inPortAddr = 0;
    if(addr==-1){   //间接寻址
        inPortAddr = cp->getRegValue(MicroCom::dx);
    }
    else{           //直接寻址
        inPortAddr = addr;
    }
    cp->setRegValue(reg,cp->readBusCycle(inPortAddr,false));
}

//输出指令OUT
void MainWindow::out(MicroCom::Regs reg, int addr){
    int outPortAddr = 0;
    int value = cp->getRegValue(reg);
    if(addr==-1){   //间接寻址
        outPortAddr = cp->getRegValue(MicroCom::dx);
    }
    else{           //直接寻址
        outPortAddr = addr;
    }
    cp->writeBusCycle(outPortAddr,value,false);
}


/** 标志位传送指令(Flag Transfers) **/
/****************************************************
 - Function：Push Flag on Stack
 - Description：对标志位无影响
 - Input：无
 - Output：SP←(SP-2)
*****************************************************/
void MainWindow::pushf(){
    int flagsValue = cp->getRegValue(MicroCom::flags);
    int stackAddr = getPhyAddr({MicroCom::sp})-2;
    int spNewValue = cp->getRegValue(MicroCom::sp)-2;
    cp->setRegValue(MicroCom::sp,spNewValue);
    cp->writeBusCycle(stackAddr,flagsValue);
    return;
}

/****************************************************
 - Function：Pop Flag off Stack
 - Description：对标志位无影响
 - Input：无
 - Output：SP←(SP+2)
*****************************************************/
void MainWindow::popf(){
    int stackAddr = this->getPhyAddr({MicroCom::sp});
    int spNewValue = cp->getRegValue(MicroCom::sp)+2;
    int flagsValue = cp->readBusCycle(stackAddr);
    cp->setRegValue(MicroCom::sp,spNewValue);
    cp->setRegValue(MicroCom::flags,flagsValue);
    return;
}


