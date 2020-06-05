#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //在MainWindow上面写软件
    MainWindow w;
    w.show();

    return a.exec();
}
