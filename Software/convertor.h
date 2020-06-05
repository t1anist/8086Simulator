#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <QWidget>
#include <QButtonGroup>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QDebug>

namespace Ui {
class Convertor;
}

class Convertor : public QWidget
{
    Q_OBJECT

public:
    explicit Convertor(QWidget *parent = nullptr);
    ~Convertor();

private:
    Ui::Convertor *ui;
    QButtonGroup *bit;
    int bitFlag;

    void setZero(int bit);
    void setValidator(int);
    int hex2(char);
    void set8v();
    void set16v();

private slots:
    void changeBit(int id);
    void hexChange();
    void signedChange();
    void unsignedChange();
    void octChange();
    void binChange();
};

#endif // CONVERTOR_H
