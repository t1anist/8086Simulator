#include "convertor.h"
#include "ui_convertor.h"

Convertor::Convertor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Convertor)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->setFixedSize(270,390);
    bit = new QButtonGroup(this);
    bit->addButton(ui->bit8);
    bit->addButton(ui->bit16);
    bit->setId(ui->bit8,0);
    bit->setId(ui->bit16,1);
    connect(bit,SIGNAL(buttonClicked(int)),this,SLOT(changeBit(int)));
    bitFlag = 0;
    setZero(bitFlag);
    set8v();
    //setValidator(bitFlag);
    connect(ui->hexLine,&QLineEdit::textChanged,this,&Convertor::hexChange);
    connect(ui->signedLine,&QLineEdit::textChanged,this,&Convertor::signedChange);
    connect(ui->unsignedLine,&QLineEdit::textChanged,this,&Convertor::unsignedChange);
    connect(ui->octLine,&QLineEdit::textChanged,this,&Convertor::octChange);
    connect(ui->binLine,&QLineEdit::textChanged,this,&Convertor::binChange);
    this->show();
}

Convertor::~Convertor()
{
    delete ui;
}

void Convertor::setZero(int bit){
    switch(bit){
        case 0:
            ui->hexLine->setText("00");
            ui->signedLine->setText("0");
            ui->unsignedLine->setText("0");
            ui->octLine->setText("0");
            ui->binLine->setText("00000000");
            break;
        case 1:
            ui->hexLine->setText("0000");
            ui->signedLine->setText("0");
            ui->unsignedLine->setText("0");
            ui->octLine->setText("0");
            ui->binLine->setText("0000000000000000");
            break;
        default: return;
    }
}

void Convertor::changeBit(int id){
    qDebug() << "id =" <<id;
    bitFlag=id;
    setZero(bitFlag);
    setValidator(bitFlag);
    qDebug() << "bitFlag =" << bitFlag;
}

void Convertor::setValidator(int id){
    if(id==0){
        QRegExp hex8("[A-Fa-f0-9]{0,2}");
        QRegExpValidator *hex8v = new QRegExpValidator(hex8,this);
        ui->hexLine->setValidator(hex8v);
        QIntValidator *signed8v = new QIntValidator(-128,127,this);
        ui->signedLine->setValidator(signed8v);
        QIntValidator *unsigned8v =new QIntValidator(0,255,this);
        ui->unsignedLine->setValidator(unsigned8v);
        QRegExp oct8("[0-3]{0,1}[0-7]{0,2}");
        QRegExpValidator *oct8v = new QRegExpValidator(oct8,this);
        ui->octLine->setValidator(oct8v);
        QRegExp bin8("[0-1]{0,8}");
        QRegExpValidator *bin8v = new QRegExpValidator(bin8,this);
        ui->binLine->setValidator(bin8v);
    }else if(id==1){
        QRegExp hex16("[A-Fa-f0-9]{0,4}");
        QRegExpValidator *hex16v = new QRegExpValidator(hex16,this);
        ui->hexLine->setValidator(hex16v);
        QIntValidator *signed16v = new QIntValidator(-32768,32767,this);
        ui->signedLine->setValidator(signed16v);
        QIntValidator *unsigned16v =new QIntValidator(0,65535,this);
        ui->unsignedLine->setValidator(unsigned16v);
        QRegExp oct16("[0-1]{0,1}[0-7]{0,5}");
        QRegExpValidator *oct16v = new QRegExpValidator(oct16,this);
        ui->octLine->setValidator(oct16v);
        QRegExp bin16("[0-1]{0,16}");
        QRegExpValidator *bin16v = new QRegExpValidator(bin16,this);
        ui->binLine->setValidator(bin16v);
    }
}

void Convertor::set8v(){
    QRegExp hex8("[A-Fa-f0-9]{0,2}");
    QRegExpValidator *hex8v = new QRegExpValidator(hex8,this);
    ui->hexLine->setValidator(hex8v);
    QIntValidator *signed8v = new QIntValidator(-128,127,this);
    ui->signedLine->setValidator(signed8v);
    QIntValidator *unsigned8v =new QIntValidator(0,255,this);
    ui->unsignedLine->setValidator(unsigned8v);
    QRegExp oct8("[0-3]{0,1}[0-7]{0,2}");
    QRegExpValidator *oct8v = new QRegExpValidator(oct8,this);
    ui->octLine->setValidator(oct8v);
    QRegExp bin8("[0-1]{0,8}");
    QRegExpValidator *bin8v = new QRegExpValidator(bin8,this);
    ui->binLine->setValidator(bin8v);
}

void Convertor::set16v(){
    QRegExp hex16("[A-Fa-f0-9]{0,4}");
    QRegExpValidator *hex16v = new QRegExpValidator(hex16,this);
    ui->hexLine->setValidator(hex16v);
    QIntValidator *signed16v = new QIntValidator(-32768,32767,this);
    ui->signedLine->setValidator(signed16v);
    QIntValidator *unsigned16v =new QIntValidator(0,65535,this);
    ui->unsignedLine->setValidator(unsigned16v);
    QRegExp oct16("[0-1]{0,1}[0-7]{0,5}");
    QRegExpValidator *oct16v = new QRegExpValidator(oct16,this);
    ui->octLine->setValidator(oct16v);
    QRegExp bin16("[0-1]{0,16}");
    QRegExpValidator *bin16v = new QRegExpValidator(bin16,this);
    ui->binLine->setValidator(bin16v);
}

void Convertor::hexChange(){
    QString hex = ui->hexLine->text();
    hex.remove(QRegExp("^(0+)"));
    int signedD,unsignedD;
    QString signedStr,unsignedStr,octStr,binStr;
    int binBit = (bitFlag==0)?8:16;
    switch(hex.length()){
        case 1: unsignedD = hex2(hex[0].toLatin1());
                signedD = unsignedD;
                break;
        case 2: unsignedD = hex2(hex[0].toLatin1()) * 16 + hex2(hex[1].toLatin1());
                if(hex2(hex[0].toLatin1())>=8){
                    signedD = -((hex2(hex[0].toLatin1())-8) * 16 + hex2(hex[1].toLatin1()));
                    if(signedD==0) signedD = -128;

                }else{
                    signedD = unsignedD;
                }
                break;
        case 3: unsignedD = hex2(hex[0].toLatin1()) * 256 + hex2(hex[0].toLatin1()) * 16 + hex2(hex[1].toLatin1());
                signedD = unsignedD;
                break;
        case 4: unsignedD = hex2(hex[0].toLatin1()) * 4096 + hex2(hex[1].toLatin1()) * 256 + hex2(hex[2].toLatin1()) * 16 + hex2(hex[3].toLatin1());
                if(hex2(hex[0].toLatin1())>=8){
                    signedD = -((hex2(hex[0].toLatin1())-8) * 4096 + hex2(hex[1].toLatin1()) * 256 + hex2(hex[2].toLatin1()) * 16 + hex2(hex[3].toLatin1()));
                    if(signedD==0) signedD = -32768;

                }else{
                    signedD = unsignedD;
                }
                break;
        default:break;
    }
    signedStr = QString::number(signedD);
    unsignedStr = QString::number(unsignedD);
    octStr = QString::number(unsignedD,8);
    binStr = QString("%1").arg(unsignedD,binBit,2,QLatin1Char('0'));
    ui->signedLine->setText(signedStr);
    ui->unsignedLine->setText(unsignedStr);
    ui->octLine->setText(octStr);
    ui->binLine->setText(binStr);
}


int Convertor::hex2(char ch){          //十六进制转换工具
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    return 0;
}

void Convertor::signedChange(){
    QString signedD = ui->signedLine->text();
    int flag;
    signedD.remove(QRegExp("^(0+)"));


}

void Convertor::unsignedChange(){

}
void Convertor::octChange(){

}
void Convertor::binChange(){

}
