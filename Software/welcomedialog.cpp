#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include <QMenu>

WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog)
{
    //load ui
    ui->setupUi(this);
    //cancel "what's this"
    this->setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    //New Button:
    connect(ui->newButton,&QToolButton::pressed,this,&WelcomeDialog::newPressed);
    //Example Button:
    QMenu *exampleMenu = new QMenu;
    exampleMenu->addAction("Hello,world");
    exampleMenu->addAction("add / subtract");
    exampleMenu->addAction("more examples");
    ui->exampleButton->setMenu(exampleMenu);
    ui->exampleButton->setPopupMode(QToolButton::InstantPopup);
    setStyleSheet("QToolButton::menu-indicator{image:none}");
    //Help Button
    connect(ui->helpButton,&QToolButton::pressed,this,&WelcomeDialog::helpPressed);
    //Recent Button
    QMenu *recentMenu = new QMenu;
    QString strPath = "../8086Software/recent.ini";
    QSettings *recent = new QSettings(strPath, QSettings::IniFormat);
    int nSize = recent->beginReadArray("OpenHistorys");
    for (int i = 0; i != nSize; ++i){
        recent->setArrayIndex(i);
        QString recentfile;
        recentfile = recent->value("path").toString();
        QFile file(recentfile);
        if (!file.exists())
            continue;
        RecentFileAction *recentAct = (RecentFileAction*)recentMenu->addAction(recentfile);
        connect(recentAct,&RecentFileAction::triggered,recentAct,&RecentFileAction::openRecent);
        void (RecentFileAction::*funcS1)(QString) = &RecentFileAction::openFile;
        void (WelcomeDialog::*funcR1)(QString) = &WelcomeDialog::openRecent;
        connect(recentAct,funcS1,this,funcR1);
    }
    recent->endArray();
    ui->recentButton->setMenu(recentMenu);
    ui->recentButton->setPopupMode(QToolButton::InstantPopup);

}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

//Button slots
void WelcomeDialog::newPressed(){
    this->close();
    emit newPage();
}
void WelcomeDialog::helpPressed(){
    this->close();
    emit helpPage();
}
void WelcomeDialog::openRecent(QString fileName){
    this->close();
    emit openFile(fileName);
}


