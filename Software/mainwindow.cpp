#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //load recent files
    loadRecent();
    //load component
    col = new QLabel(this);
    line = new QLabel(this);
    col->setText(" col:0 ");
    line->setText(" line:0 ");
    ui->statusbar->addWidget(line);
    ui->statusbar->addWidget(col);
    setQscintilla();
    setCentralWidget(editor);
    setCurrentFile("");
    this->welDialog = new WelcomeDialog(this);
    this->connectAction();
    this->show();
    welDialog->show();
}

void MainWindow::closeEvent(QCloseEvent *event){
    if (maybeSave()){
        event->accept();
    } else {
        event->ignore();
    }
}

MainWindow::~MainWindow()
{
    QString strPath = "../8086Software/recent.ini";
    recent = new QSettings(strPath, QSettings::IniFormat);
    recent->beginWriteArray("OpenHistorys");
    for (int i = 0; i != listOpenHistory.size(); ++i){
        recent->setArrayIndex(i);
        recent->setValue("path",listOpenHistory[i]);
    }
    recent->endArray();
    delete recent;
    delete ui;
}

void MainWindow::loadRecent(){
    QString strPath = "../8086Software/recent.ini";
    recent = new QSettings(strPath, QSettings::IniFormat);
    int nSize = recent->beginReadArray("OpenHistorys");
    for (int i = 0; i != nSize; ++i){
        recent->setArrayIndex(i);
        QString recentfile;
        recentfile = recent->value("path").toString();
        QFile file(recentfile);
        if (!file.exists())
            continue;
        listOpenHistory.enqueue(recentfile);
    }
    recent->endArray();
    updateRecent();
}

void MainWindow::updateRecent(){
    ui->menurecent_files->clear();
    if (listOpenHistory.size()==0){
            ui->menurecent_files->setEnabled(false);
            return;
    }else{
        ui->menurecent_files->setEnabled(true);
        for (int i = 0; i != listOpenHistory.size(); ++i){
            RecentFileAction *recentAct = dynamic_cast<RecentFileAction*>(ui->menurecent_files->addAction(listOpenHistory[i]));

            connect(recentAct,&RecentFileAction::triggered,recentAct,&RecentFileAction::openRecent);
            void (RecentFileAction::*funcS1)(QString) = &RecentFileAction::openFile;
            void (MainWindow::*funcR1)(QString) = &MainWindow::open;
            connect(recentAct,funcS1,this,funcR1);
        }
        ui->menurecent_files->addSeparator();
        QAction *clear = ui->menurecent_files->addAction("clear list");
        connect(clear,&QAction::triggered,this,&MainWindow::clearRecent);
        ui->menurecent_files->update();
    }
}

void MainWindow::setQscintilla(){
    editor=new QsciScintilla(this);
    editor->SendScintilla(QsciScintilla::SCI_SETLEXER,QsciScintilla::SCLEX_ASM);
    //设置语法
    QsciLexerCPP *textLexer=new QsciLexerCPP;//创建一个词法分析器
    editor->setLexer(textLexer); //给QsciScintilla设置词法分析器
    textLexer->setPaper(QColor(Qt::white));//文本区域背景色
    textLexer->setColor(QColor(0,170,0),QsciLexerCPP::Comment); //设置自带的注释行为灰色
    //代码提示
  //  QsciAPIs *apis=new QsciAPIs(textLexer);
//    if(!apis->load(QString("D://8086//8086Software//8086Software//docapi.txt"))){
//        QMessageBox::warning(this,QString("提示"),QString("读取文件失败"));
//    }else{
//        apis->prepare();
//    }
   // apis->add("MOV");
    //apis->prepare();
    //行号提示
    editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE,QsciScintilla::SC_CP_UTF8);//设置编码为UTF-8
    QFont line_font;
    line_font.setFamily("SimSun");
    line_font.setPointSize(11);
    editor->setFont(line_font);//设置文本字体
    //editor->setWrapMode(QsciScintilla::WrapWord); //文本自动换行模式
    //editor->setWrapVisualFlags(QsciScintilla::WrapFlagByText);
    editor->setEolMode(QsciScintilla::EolWindows); //微软风格换行符
    //editor->setEolVisibility(true);//显示换行符
    //editor->setWhitespaceVisibility(QsciScintilla::WsVisible);//此时空格为点，\t为箭头
    editor->setWhitespaceVisibility(QsciScintilla::WsInvisible);
    //editor->setWhitespaceSize(2);//空格点大小

        //indent
        editor->setIndentationsUseTabs(false);//false用表示用空格代替\t
        editor->setTabWidth(4);//\t宽度设为四个空格
        editor->setIndentationGuides(false);//用tab键缩进时，在缩进位置上显示一个竖点线，缩进有效，在字符串后加空格不显示
        editor->setIndentationWidth(0);//如果在行首部空格位置tab，缩进的宽度字符数，并且不会转换为空格
        editor->setAutoIndent(true);//换行后自动缩进
        editor->setBackspaceUnindents(true);
        editor->setTabIndents(true);//True如果行前空格数少于tabWidth，补齐空格数,False如果在文字前tab同true，如果在行首tab，则直接增加tabwidth个空格
        editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);//括号匹配

        //current line color
        editor->setCaretWidth(2);//光标宽度，0表示不显示光标
        editor->setCaretForegroundColor(QColor("darkCyan"));  //光标颜色
        editor->setCaretLineVisible(false); //是否高亮显示光标所在行
        editor->setCaretLineBackgroundColor(QColor(100,250,100));//光标所在行背景颜色

        //selection color
        editor->setSelectionBackgroundColor(Qt::black);//选中文本背景色
        editor->setSelectionForegroundColor(Qt::white);//选中文本前景色

        //edge
        //editor->setEdgeColumn(50); //edge宽度，没发现有啥用
        //editor->setEdgeMode(QsciScintilla::EdgeLine);
        //editor->setEdgeColor(QColor("green"));

        QFont margin_font;
        margin_font.setFamily("SimSun");
        margin_font.setPointSize(11);//边栏字体设置px我这里显示不出行号，不知道是怎么回事
        editor->setMarginsFont(margin_font);//设置页边字体
        editor->setMarginType(0,QsciScintilla::NumberMargin);//设置标号为0的页边显示行号
        //editor->setMarginMarkerMask(0,QsciScintilla::Background);//页边掩码
        //editor->setMarginSensitivity(0,true);//注册通知事件，当用户点击边栏时，scintilla会通知我们
        editor->setMarginLineNumbers(0,true);//设置第0个边栏为行号边栏，True表示显示
        editor->setMarginWidth(0,30);//设置0边栏宽度
        editor->setMarginsBackgroundColor(Qt::gray);
        editor->setMarginsForegroundColor(Qt::white);

        editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);//折叠样式
        editor->setFoldMarginColors(Qt::gray,Qt::lightGray);//折叠栏颜色

        //auto complete
        //Acs[None|All|Document|APIs]
        //禁用自动补全提示功能|所有可用的资源|当前文档中出现的名称都自动补全提示|使用QsciAPIs类加入的名称都自动补全提示
        editor->setAutoCompletionSource(QsciScintilla::AcsAll);//自动补全。对于所有Ascii字符
        editor->setAutoCompletionCaseSensitivity(false);//大小写敏感度，设置lexer可能会更改，不过貌似没啥效果
        editor->setAutoCompletionThreshold(1);//设置每输入一个字符就会出现自动补全的提示
        //editor->setAutoCompletionReplaceWord(false);//是否用补全的字符串替代光标右边的字符串
        setCentralWidget(editor);
}

void MainWindow::connectAction(){
    //WelcomeDialog connect
    connect(welDialog,&WelcomeDialog::newPage,this,&MainWindow::openNewPage);
    connect(welDialog,&WelcomeDialog::helpPage,this,&MainWindow::help);
    void (MainWindow::*func1)(QString) = &MainWindow::open;
    connect(welDialog,&WelcomeDialog::openFile,this,func1);
    connect(editor, SIGNAL(cursorPositionChanged(int,int)),this, SLOT(slotEditorCursorChange(int,int)));
    connect(editor, SIGNAL(textChanged()),this, SLOT(documentWasModified()));

    //Menu Connect
    //1.file
    connect(ui->actioncom_template,&QAction::triggered,this,&MainWindow::openComTemplate);
    connect(ui->actionexe_template,&QAction::triggered,this,&MainWindow::openExeTemplate);
    connect(ui->actionmore_examples,&QAction::triggered,this,&MainWindow::openExamples);
    connect(ui->actionopen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionsave,&QAction::triggered,this,&MainWindow::save);
    connect(ui->actionsave_as,&QAction::triggered,this,&MainWindow::saveAs);
    connect(ui->actionexit,&QAction::triggered,this,&MainWindow::close);

    //2.edit
    connect(ui->actionundo,&QAction::triggered,editor,&QsciScintilla::undo);
    connect(ui->actionredo,&QAction::triggered,editor,&QsciScintilla::redo);
    connect(ui->actioncut,&QAction::triggered,editor,&QsciScintilla::cut);
    connect(ui->actioncopy,&QAction::triggered,editor,&QsciScintilla::copy);
    connect(ui->actionpaste,&QAction::triggered,editor,&QsciScintilla::paste);
    ui->actioncut->setEnabled(false);
    ui->actioncopy->setEnabled(false);
    connect(editor, SIGNAL(copyAvailable(bool)),ui->actioncut, SLOT(setEnabled(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)),ui->actioncopy, SLOT(setEnabled(bool)));
    connect(ui->actionselect_all,&QAction::triggered,this,&MainWindow::selectAllSlot);
    connect(this,&MainWindow::selectAllSignal,editor,&QsciScintilla::selectAll);
    connect(ui->actiongo_to_line,&QAction::triggered,this,&MainWindow::goToLine);

    //3.assembler

    //4.math
    connect(ui->actionbase_convertor,&QAction::triggered,this,&MainWindow::openConvertor);
    connect(ui->actionASCII_codes,&QAction::triggered,this,&MainWindow::ASCII);


    //Tool Connect
    connect(ui->actionnew,&QAction::triggered,this,&MainWindow::openNewPage);
    connect(ui->actionopen_2,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionsave_2,&QAction::triggered,this,&MainWindow::save);
    connect(ui->actionsave_as_2,&QAction::triggered,this,&MainWindow::saveAs);
    connect(ui->actionconvertor,&QAction::triggered,this,&MainWindow::openConvertor);
}

void MainWindow::openTemplate(int type){
    switch(type){
        case 0: openComTemplate();
            break;
        case 1: openExeTemplate();
            break;
        case 2: newFile();
            break;
        case 3: openEmulator();
            break;
        default: break;
    }
}

void MainWindow::openNewPage(){
    if(welDialog){
        delete welDialog;
        welDialog = NULL;
    }
    newPage = new NewDialog(this);
    connect(newPage,SIGNAL(sendType(int)),this,SLOT(openTemplate(int)));
    newPage->exec();   
}


void MainWindow::openComTemplate(){
    QString fileName = "../8086Software/doc/com.asm";
    if (!fileName.isEmpty()){
        loadFile(fileName);
        isFixedFile = true;
    }
}

void MainWindow::openExeTemplate(){
    QString fileName = "../8086Software/doc/exe.asm";
    if (!fileName.isEmpty()){
        loadFile(fileName);
        isFixedFile = true;
    }
}

void MainWindow::openEmulator(){

}


void MainWindow::openExamples(){
    QString fileName = QFileDialog::getOpenFileName(this,"Open File","../8086Software/examples",
                                                    "All known files(*.asm *.exe *.com);;"
                                                    "Assembly source files(*.asm);;"
                                                    "Binary execute files(*.com);;"
                                                    "Execute files(*.exe);;"
                                                    "All files(*.*)");
    if (!fileName.isEmpty())
        loadFile(fileName);
}


void MainWindow::help(){
    qDebug()<< "help";
}

void MainWindow::slotEditorCursorChange(int lin,int index){
    col->setText(QString("col:%1").arg(index));
    line->setText(QString("line:%1").arg(lin+1));
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        editor->clear();
        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,"Open File","../8086Software/examples",
                                                        "All known files(*.asm *.exe *.com);;"
                                                        "Assembly source files(*.asm);;"
                                                        "Binary execute files(*.com);;"
                                                        "Execute files(*.exe);;"
                                                        "All files(*.*)");
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

void MainWindow::open(QString fileName)
{
    if (maybeSave()) {
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()||isFixedFile) {
        return saveAs();
    } else {
        bool isRecent = false;
        for (int i = 0 ; i != listOpenHistory.size();i++) {
            if(curFile==listOpenHistory[i]){
                isRecent = true;
                break;
            }
        }
        if(!isRecent){
            if(listOpenHistory.size()>10){
                listOpenHistory.dequeue();
            }
            listOpenHistory.enqueue(curFile);
        }
        updateRecent();
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Save File","../8086Software/files",
                                                    "Assembly source files(*.asm);;"
                                                    "All known files(*.asm *.exe *.com);;"
                                                    "Binary execute files(*.com);;"
                                                    "Execute files(*.exe);;"
                                                    "All files(*.*)");
    if (fileName.isEmpty())
        return false;
    bool isRecent = false;
    for (int i = 0 ; i != listOpenHistory.size();i++) {
        if(fileName==listOpenHistory[i]){
            isRecent = true;
            break;
        }
    }
    if(!isRecent){
        if(listOpenHistory.size()>10){
            listOpenHistory.dequeue();
        }
        listOpenHistory.enqueue(fileName);
    }
    updateRecent();
    return saveFile(fileName);
}
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    editor->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty()){
        shownName = "untitled.asm";
    }else{
        shownName = strippedName(curFile);
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("8086Software")));
}

bool MainWindow::maybeSave()
{
    if (editor->isModified()) {
        int ret = QMessageBox::warning(this, "8086Software Exit",
                     "The document has been modified.\n"
                        "Do you want to save your changes?",
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);

        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Load File Error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    editor->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Save File Error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << editor->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::documentWasModified(){
    setWindowModified(editor->isModified());
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::clearRecent(){
    int size = listOpenHistory.size();
    for (int i = 0;i<size;i++) {
        listOpenHistory.dequeue();
    }
    QString strPath = "../8086Software/recent.ini";
    recent = new QSettings(strPath, QSettings::IniFormat);
    recent->beginWriteArray("OpenHistorys");
    for (int i = 0; i < 10; ++i){
        recent->setArrayIndex(i);
        recent->setValue("path","null");
    }
    recent->endArray();
    updateRecent();
}

void MainWindow::selectAllSlot(){
    emit selectAllSignal(true);
}

void MainWindow::goToLine(){
    goToLineDialog = new GoToLineDialog(this);
    connect(goToLineDialog,&GoToLineDialog::goToLineNum,this,&MainWindow::goToLineNum);
}

void MainWindow::goToLineNum(int num){
    editor->setCursorPosition(num-1,0);
}


void MainWindow::openConvertor(){
    convertor = new Convertor();
}

void MainWindow::ASCII(){

}

