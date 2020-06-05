#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = nullptr);
    ~NewDialog();


private:
    Ui::NewDialog *ui;
    QButtonGroup *buttonGroup;

private slots:
    void ok();

signals:
    void sendType(int id);

};

#endif // NEWDIALOG_H
