#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include "CPUs.h"
#include "Latchs.h"
#include "PPIs.h"
#include "Decoders.h"
#include "LogicGate.h"
#include "Buffers.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /** =================== 数据传送指令 ================== **/

    /** mov指令 **/
    //immediate value to Register
    void mov(MicroCom::Regs target, int imValue);
    //immediate value to Memory
    void mov(MemoryUnit m, int imValue);
    //Register to Register
    void mov(MicroCom::Regs target, MicroCom::Regs source);    
    //Memory to Register & Register to Memory
    void mov(MicroCom::Dir, MicroCom::Regs reg, MemoryUnit m);

    /** in/out指令 **/
    void in(MicroCom::Regs reg, int addr=-1);
    void out(MicroCom::Regs reg, int addr=-1);

    /** 出入栈指令 **/
    //push Register on stack
    void push(MicroCom::Regs source);
    //push Memory on stack
    void push(MemoryUnit m);
    //pop stack to Register
    void pop(MicroCom::Regs target);
    //pop stack to Memory
    void pop(MemoryUnit m);

    /** XCHG交换指令 **/
    //Register to Memory & Memory to Register
    void xChg(MicroCom::Dir, MicroCom::Regs reg, MemoryUnit m);
    //Register to Register
    void xChg(MicroCom::Regs target, MicroCom::Regs source);

    /** 地址目标传送指令(Address Object Transfers) **/
    /* 该类指令要求源操作数只能是存储单元,且目的操作数只能是寄存器 */
    // LEA取地址有效指令(Load Effective Address)
    void lea(MicroCom::Regs target, MemoryUnit m);
    // LDS将双字指针送到寄存器和DS指令(Load Pointer Using DS)
    void lds(MicroCom::Regs target, MemoryUnit m);
    // LES将双字指针送到寄存器和ES指令(Load Pointer Using ES)
    void les(MicroCom::Regs target, MemoryUnit m);

    /** 标志位传送指令(Flag Transfers) **/
    //PUSHF标志入栈指令(Push Flag onto Stack)
    void pushf();
    //POPF标志出栈指令(Pop Flag off Stack)
    void popf();

    /** =================== 算术运算指令 ================== **/

    /** 加法指令(Addition) **/
    // ADD加法指令(Addition) & ADC带进位的加法指令
    //immediate value to Register
    void add(MicroCom::Regs target, int imValue);
    //immediate value to Memory
    void add(int imValue, MemoryUnit m, bool isCarry);
    //Register to Register
    void add(MicroCom::Regs target, MicroCom::Regs source);
    //Memory to Register & Register to Memory
    void add(MicroCom::Dir, MicroCom::Regs reg, MemoryUnit m, bool isCarry=false);

    /** 寻址指令 **/
    int getPhyAddr(MemoryUnit m);

    //连线函数，将两个引脚连接起来
    void link(Hardwares *sender, MicroCom::Pins pinS, Hardwares *receiver, MicroCom::Pins pinR);

    //Input and Output


private:
    Ui::MainWindow *ui;
    CPUs* cp;
    Latchs* la1;
    Latchs* la2;
    Latchs* la3;
    Buffers *bf1;
    Buffers *bf2;
    Decoders* de1;
    PPIs* pp1;
    LogicGate* nand;
};

#endif // MAINWINDOW_H
