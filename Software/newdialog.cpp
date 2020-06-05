#include "newdialog.h"
#include "ui_newdialog.h"

NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->com);
    buttonGroup->addButton(ui->exe);
    buttonGroup->addButton(ui->empty);
    buttonGroup->addButton(ui->emulator);
    buttonGroup->setId(ui->com,0);
    buttonGroup->setId(ui->exe,1);
    buttonGroup->setId(ui->empty,2);
    buttonGroup->setId(ui->emulator,3);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&NewDialog::ok);
}

void NewDialog::ok(){
    int id = buttonGroup->checkedId();
    emit sendType(id);
    this->close();
}

NewDialog::~NewDialog()
{
    delete ui;
}


