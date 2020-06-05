#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H

#include <QDialog>

namespace Ui {
class GoToLineDialog;
}

class GoToLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoToLineDialog(QWidget *parent = nullptr);
    ~GoToLineDialog();

private:
    Ui::GoToLineDialog *ui;

private slots:
    void ok();

signals:
    void goToLineNum(int);
};

#endif // GOTOLINEDIALOG_H
