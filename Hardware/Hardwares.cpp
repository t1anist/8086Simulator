#include "Hardwares.h"

Hardwares::Hardwares(QWidget *parent) : QWidget(parent)
{

}

void Hardwares::setHardwareName(QString hdName){
    hardwareName = hdName;
}


/****************************************************
 - Function：get the hardware's name
 - Description：
 - Input：
 - Return：hardware's name(QString)
*****************************************************/
QString Hardwares::getHardwareName(){
    return hardwareName;
}

/****************************************************
 - Function：delay or sleep
 - Description：非阻塞延时
 - Input：msec（毫秒）
 - Return：
*****************************************************/
void Hardwares::delaymsec(int msec){
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
