#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QQueue>

#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexercpp.h"
#include "Qsci/qsciapis.h"

#include "welcomedialog.h"
#include "newdialog.h"
#include "recentfileaction.h"
#include "gotolinedialog.h"
#include "convertor.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //Widgets
    Ui::MainWindow *ui;
    WelcomeDialog *welDialog;
    NewDialog *newPage;
    GoToLineDialog *goToLineDialog;
    Convertor *convertor;
    QsciScintilla *editor;
    QString curFile;
    QLabel *col;
    QLabel *line;
    QSettings *recent;
    QQueue<QString> listOpenHistory;

    void loadRecent();
    //save setting
    bool isFixedFile = false;
    void setQscintilla();
    void connectAction();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void openTemplateType(int type);
    void openEmulator();
    void updateRecent();



private slots:
    void openNewPage();
    void openTemplate(int);
    void newFile();
    void open();
    void open(QString);
    bool save();
    bool saveAs();
    void documentWasModified();
    void openComTemplate();
    void openExeTemplate();
    void openExamples();
    void slotEditorCursorChange(int lin,int index);
    void help();
    void clearRecent();
    void selectAllSlot();
    void goToLine();
    void goToLineNum(int);
    void openConvertor();
    void ASCII();

signals:
    void selectAllSignal(bool);


protected:
    void closeEvent(QCloseEvent *event) override;


};
#endif // MAINWINDOW_H
