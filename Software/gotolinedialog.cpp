#include "gotolinedialog.h"
#include "ui_gotolinedialog.h"

GoToLineDialog::GoToLineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToLineDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&GoToLineDialog::ok);
    this->show();
}

GoToLineDialog::~GoToLineDialog()
{
    delete ui;
}

void GoToLineDialog::ok(){
    int line = ui->lineNum->value();
    emit goToLineNum(line);
}
