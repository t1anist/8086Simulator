#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <newdialog.h>
#include <QDialog>
#include <QSettings>
#include <QFile>

#include "recentfileaction.h"


namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

private:
    Ui::WelcomeDialog *ui;
    NewDialog *newDialog;
    QStringList listOpenHistory;

private slots:
    //Button slots
    void newPressed();
    void helpPressed();
    void openRecent(QString);

signals:
    //signals to mainwindow
    void newPage();
    void helpPage();
    void openFile(QString);

};

#endif // WELCOMEDIALOG_H
