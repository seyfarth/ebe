#include <QtGui>
#include <QTimer>
#include <QPushButton>
#include <QtWebKit>
#include <QWebView>
#include <QApplication>
#include <QKeySequence>
#include <QMessageBox>
#ifdef Q_WS_WIN
#include <windows.h>
#else
#include <unistd.h>
#include <signal.h>
#endif

#include "mainwindow.h"
#include "errorwindow.h"
#include "librarywindow.h"
#include "sourcewindow.h"
#include "settings.h"
#include "stylesheet.h"
#include "instructions.h"

extern bool userSetGeometry;
extern int  userWidth;
extern int  userHeight;

DataWindow *dataWindow;
SourceFrame *sourceFrame;
RegisterWindow *registerWindow;
FloatWindow *floatWindow;
ProjectWindow *projectWindow;
TerminalWindow *terminalWindow;
//ConsoleWindow *consoleWindow;
BackTraceWindow *backTraceWindow;
BitBucket *bitBucket;
GDB *gdb;
GDBThread *gdbThread;
ToyBox *toyBox;

QStatusBar *statusBar;

QToolBar *fileToolBar;
QToolBar *editToolBar;
QToolBar *debugToolBar;
QToolBar *templateToolBar;

extern QProcess *gdbProcess;

#ifdef Q_WS_WIN
extern HANDLE hProcess;
extern bool needToKill;
#else
extern int gdbWaiting;
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    settings = new Settings;
    settings->read();

#ifdef Q_WS_WIN
    QProcess where(this);
    where.start("where objdump");
    where.waitForFinished();
    QByteArray data = where.readAllStandardOutput();
    QString fileName;
    fileName = QString(data.trimmed());
    where.start(QString("objdump -f \"%1\"").arg(fileName));
    where.waitForFinished();
    data = where.readAllStandardOutput();
    if ( QString(data).indexOf("x86-64") >= 0 ) {
        wordSize = 64;
    } else {
        wordSize = 32;
    }
#endif
#ifdef Q_OS_LINUX
    QProcess which(this);
    which.start("which objdump");
    which.waitForFinished();
    QByteArray data = which.readAllStandardOutput();
    QString fileName;
    fileName = QString(data.trimmed());
    which.start(QString("objdump -f \"%1\"").arg(fileName));
    which.waitForFinished();
    data = which.readAllStandardOutput();
    if ( QString(data).indexOf("x86-64") >= 0 ) {
        wordSize = 64;
    } else {
        wordSize = 32;
    }
#endif
#ifdef Q_OS_MAC
    QProcess which(this);
    which.start("which nm");
    which.waitForFinished();
    QByteArray data = which.readAllStandardOutput();
    QString fileName;
    fileName = QString(data.trimmed());
    which.start(QString("file \"%1\"").arg(fileName));
    which.waitForFinished();
    data = which.readAllStandardOutput();
    if ( QString(data).indexOf("x86_64") >= 0 ) {
        wordSize = 64;
    } else {
        wordSize = 32;
    }
#endif

    //qDebug() << wordSize;

    qRegisterMetaType<QList<StringSet> >("QList<StringSet>");
    qRegisterMetaType<StringHash>("StringHash");
    qRegisterMetaType<QHash<QString,ClassDefinition> >("QHash<QString,ClassDefinition>");
    qApp->installEventFilter(this);
    
    gdbThread = new GDBThread();
    gdbThread->start();
#ifdef Q_WS_WIN
    while ( !gdb ) Sleep(1);
#else
    while ( !gdb ) usleep(10);
#endif
    //qDebug() << "gdb" << gdb;

    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("ebe"));
    sourceFrame = new SourceFrame(this);
    setCentralWidget(sourceFrame);

    createMenus();

    readInstructions();

    setUnifiedTitleAndToolBarOnMac(false);

    QTimer::singleShot(0,this,SLOT(restoreMainWindow()));

    connect ( gdb, SIGNAL(sendRegs(StringHash)),
              registerWindow, SLOT(receiveRegs(StringHash)) );
    connect ( gdb, SIGNAL(sendFpRegs(QStringList)),
              floatWindow, SLOT(receiveFpRegs(QStringList)) );
    connect ( this, SIGNAL(sendWorkingDir(QString)),
              gdb, SLOT(receiveWorkingDir(QString)) );
    connect ( this, SIGNAL(doStop()), gdb, SLOT(doStop()) );
    restoreMainWindow();
}

void MainWindow::closeEvent ( QCloseEvent *event )
{
    if ( sourceFrame->filesSaved() ) {
#ifdef Q_WS_WIN
        if ( needToKill ) TerminateProcess(hProcess,0);
#endif
        saveSettings();
        event->accept();
    } else {
        event->ignore();
    }
}


void MainWindow::restoreMainWindow()
{
    tooltipsVisible = ebe["tooltips/visible"].toBool();

    addStyleSheet("textedit-font",
            QString("QTextEdit { font-weight: bold; font-family: %1}").
            arg(ebe["fixed_font"].toString()));
    addStyleSheet("plaintextedit-font",
            QString("QPlainTextEdit { font-weight: bold; font-family: %1}").
            arg(ebe["fixed_font"].toString()));
    addStyleSheet("plaintextedit-bg",
            QString("QPlainTextEdit { background: %1}").
            arg(ebe["bg_color"].toString()));
    addStyleSheet("table-bg",
            QString("QTableWidget { background: %1}").
            arg(ebe["table_bg"].toString()));
    addStyleSheet("list-bg",
            QString("QListWidget { background: %1}").
            arg(ebe["list_bg"].toString()));
    addStyleSheet("tree-bg",
            QString("QTreeWidget { background: %1}").
            arg(ebe["tree_bg"].toString()));
    addStyleSheet("lineedit-font",
            QString("QLineEdit { font-weight: bold; font-family: %1}").
            arg(ebe["fixed_font"].toString()));
    addStyleSheet("table-font",
            QString("QTableView { font-weight: bold; font-family: %1}").
            arg(ebe["fixed_font"].toString()));
    addStyleSheet("list-font",
            QString("QListWidget { font-weight: bold; font-family: %1}").
            arg(ebe["fixed_font"].toString()));
    addStyleSheet("tree-font",
            QString("QTreeWidget { font-weight: bold; font-family: %1}").
            arg(ebe["fixed_font"].toString()));
    addStyleSheet("tab-font",
            QString("QTabBar { font-family: %1}").
            arg(ebe["variable_font"].toString()));

    dataDock->setFloating(ebe["data/floating"].toBool());
    registerDock->setFloating(ebe["register/floating"].toBool());
    floatDock->setFloating(ebe["float/floating"].toBool());
    projectDock->setFloating(ebe["project/floating"].toBool());
    terminalDock->setFloating(ebe["terminal/floating"].toBool());
    toyBoxDock->setFloating(ebe["toybox/floating"].toBool());
    bitBucketDock->setFloating(ebe["bitbucket/floating"].toBool());
    //consoleDock->setFloating(ebe["console/floating"].toBool());

    if ( !userSetGeometry &&  ebe.contains("ebe/geometry") ) {
        restoreGeometry(ebe["ebe/geometry"].toByteArray());
    } else if ( userSetGeometry ) {
        resize(userWidth,userHeight);
    } else {
        resize(1000,750);
    }
    if ( !userSetGeometry && ebe.contains("ebe/state") ) {
        restoreState(ebe["ebe/state"].toByteArray());
    } else if ( userSetGeometry ) {
        resize(userWidth,userHeight);
    } else {
        resize(1000,750);
    }
    dataDock->setStyleSheet("QDockWidget::title { font-family: " +
                            ebe["variable_font"].toString() + "}" );
    registerDock->setStyleSheet("QDockWidget::title { font-family: " +
                            ebe["variable_font"].toString() + "}" );
    floatDock->setStyleSheet("QDockWidget::title { font-family: " +
                            ebe["variable_font"].toString() + "}" );
    projectDock->setStyleSheet("QDockWidget::title { font-family: " +
                            ebe["variable_font"].toString() + "}" );
    terminalDock->setStyleSheet("QDockWidget::title { font-family: " +
                            ebe["variable_font"].toString() + "}" );
    //consoleDock->setStyleSheet("QDockWidget::title { font-family: " +
                            //ebe["variable_font"].toString() + "}" );

    fontSize = ebe["font_size"].toInt();
    setFontSize();
}

bool MainWindow::eventFilter ( QObject * /* object */, QEvent *event )
{
    if ( event->type() == QEvent::ToolTip ) {
        if ( !tooltipsVisible ) return true;
    }
    return false;
}

void MainWindow::saveSettings()
{
    ebe["ebe/geometry"]      = saveGeometry();
    ebe["ebe/state"]         = saveState();
    ebe["data/floating"]     = dataDock->isFloating();
    ebe["register/floating"] = registerDock->isFloating();
    ebe["float/floating"]    = floatDock->isFloating();
    ebe["project/floating"]  = projectDock->isFloating();
    ebe["terminal/floating"] = terminalDock->isFloating();
    ebe["toybox/floating"] = toyBoxDock->isFloating();
    ebe["bitbucket/floating"] = bitBucketDock->isFloating();
    //ebe["console/floating"]  = consoleDock->isFloating();
    ebe["data/visible"]      = dataDock->isVisible();
    ebe["register/visible"]  = registerDock->isVisible();
    ebe["float/visible"]     = floatDock->isVisible();
    ebe["project/visible"]   = projectDock->isVisible();
    ebe["terminal/visible"]  = terminalDock->isVisible();
    ebe["toybox/visible"]  = toyBoxDock->isVisible();
    ebe["bitbucket/visible"]  = bitBucketDock->isVisible();
    //ebe["console/visible"]   = consoleDock->isVisible();
    settings->write();
}

void MainWindow::setFontSize()
{
    int width;
    int height;
    if ( fontSize < 4 ) fontSize = 4;
    if ( fontSize > 100 ) fontSize = 100;
    ebe["font_size"] = fontSize;
    addStyleSheet("font-size", "* {font-size: " + QString("%1").arg(fontSize) + "px}");
    //addStyleSheet("table-font-size", "QTableView {font-size: " + QString("%1").arg(fontSize) + "px}");
    QFont f("courier");
    f.setPixelSize(fontSize);
    QFontMetrics fm(f);
    width = fm.width("x");
    height = fm.height();
    sourceFrame->setFontHeightAndWidth(height,width);
    dataWindow->setFontHeightAndWidth(height,width);
    floatWindow->setFontHeightAndWidth(height,width);
    registerWindow->setFontHeightAndWidth(height,width);
    toyBox->setFontHeightAndWidth(height,width);
    bitBucket->setFontHeightAndWidth(height,width);
}

void MainWindow::increaseFont()
{

    fontSize++;
    setFontSize();
}

void MainWindow::decreaseFont()
{
    fontSize--;
    setFontSize();
}

void MainWindow::createMenus()
{
    int icon_size = ebe["toolbars/icon_size"].toInt();

    fileToolBar = new QToolBar("File toolbar",this);
    fileToolBar->setObjectName("File toolbar");
    fileToolBar->setIconSize(QSize(icon_size,icon_size));

    editToolBar = new QToolBar("Edit toolbar",this);
    editToolBar->setObjectName("Edit toolbar");
    editToolBar->setIconSize(QSize(icon_size,icon_size));

    debugToolBar = new QToolBar("Debug toolbar",this);
    debugToolBar->setObjectName("Debug toolbar");
    debugToolBar->setIconSize(QSize(icon_size,icon_size));

    templateToolBar = new QToolBar("Code template toolbar",this);
    templateToolBar->setObjectName("Template toolbar");
    templateToolBar->setIconSize(QSize(icon_size,icon_size));

    menuBar()->setStyleSheet("font-family: "+ebe["variable_font"].toString());
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("New"), sourceFrame,
                        SLOT(newFile()), QKeySequence::New );
    templateMenu = fileMenu->addMenu(tr("&Template"));
        templateMenu->addAction(tr("C Program"),
                       sourceFrame, SLOT(templateC()));
        templateMenu->addAction(tr("C++ Program"), sourceFrame, SLOT(templateCpp()));
        templateMenu->addAction(tr("Assembly Program"), sourceFrame,
                      SLOT(templateAssembly()));
        templateMenu->addAction(tr("Fortran Program"), sourceFrame, SLOT(templateFortran()));
    fileMenu->addAction(tr("Open"), sourceFrame, SLOT(open(bool)), QKeySequence::Open );
    fileMenu->addAction(tr("Save"), sourceFrame, SLOT(save()), QKeySequence::Save );
    fileMenu->addAction(tr("Save as"), sourceFrame, SLOT(saveAs()) );
    fileMenu->addAction(tr("Close"), sourceFrame, SLOT(close()) );
    fileMenu->addAction(tr("Change directory"), this, SLOT(changeDirectory()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("New project"), projectWindow, SLOT(newProject()) );
    fileMenu->addAction(tr("Open project"), projectWindow, SLOT(openProject()) );
    fileMenu->addAction(tr("Close project"), projectWindow, SLOT(closeProject()) );
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Quit"), this, SLOT(quit()), QKeySequence::Quit );

    fileToolBar->addAction(QIcon(QString(":/icons/%1/quit.png").arg(icon_size)),
        tr("Quit"), this, SLOT(quit()) );
    fileToolBar->addSeparator();
    fileToolBar->addAction(QIcon(QString(":/icons/%1/new.png").arg(icon_size)),
        tr("New"), sourceFrame, SLOT(newFile()) );
    fileToolBar->addAction(QIcon(QString(":/icons/%1/open.png").arg(icon_size)),
        tr("Open"), sourceFrame, SLOT(open(bool)) );
    fileToolBar->addAction(QIcon(QString(":/icons/%1/save.png").arg(icon_size)),
        tr("Save"), sourceFrame, SLOT(save()) );
    fileToolBar->addAction(QIcon(QString(":/icons/%1/save-as.png").arg(icon_size)),
        tr("Save as"), sourceFrame, SLOT(saveAs()) );
    fileToolBar->addAction(QIcon(QString(":/icons/%1/close.png").arg(icon_size)),
        tr("Close"), sourceFrame, SLOT(close()) );

    fileToolBar->addSeparator();
    fileToolBar->addAction(QIcon(QString(":/icons/%1/project-new.png").
        arg(icon_size)),
        tr("New project"), projectWindow, SLOT(newProject()) );
    fileToolBar->addAction(QIcon(QString(":/icons/%1/project-open.png").
        arg(icon_size)),
        tr("Open project"), projectWindow, SLOT(openProject()) );
    fileToolBar->addAction(QIcon(QString(":/icons/%1/project-close.png").
        arg(icon_size)),
        tr("Close project"), projectWindow, SLOT(closeProject()) );

    addToolBar(Qt::TopToolBarArea,fileToolBar);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(tr("Cut"), sourceFrame, SLOT(cut()), QKeySequence::Cut );
    editMenu->addAction(tr("Copy"), sourceFrame, SLOT(copy()), QKeySequence::Copy );
    editMenu->addAction(tr("Paste"), sourceFrame, SLOT(paste()), QKeySequence::Paste );
    editMenu->addSeparator();
    editMenu->addAction(tr("Undo"), sourceFrame, SLOT(undo()), QKeySequence::Undo );
    editMenu->addAction(tr("Redo"), sourceFrame, SLOT(redo()), QKeySequence::Redo );
    editMenu->addSeparator();
    editMenu->addAction(tr("Comment"), sourceFrame, SLOT(comment()),
                        QKeySequence("Ctrl+K") );
    editMenu->addAction(tr("Uncomment"), sourceFrame, SLOT(unComment()),
                        QKeySequence("Ctrl+U")  );
    editMenu->addAction(tr("indent"), sourceFrame, SLOT(indent()),
                        QKeySequence("Ctrl+>")  );
    editMenu->addAction(tr("Unindent"), sourceFrame, SLOT(unIndent()),
                        QKeySequence("Ctrl+<")  );
    editMenu->addSeparator();
    editMenu->addAction(tr("Find"), sourceFrame, SLOT(find()), QKeySequence::Find );
    editMenu->addSeparator();
    editMenu->addAction(tr("Select all"), sourceFrame, SLOT(selectAll()),
                        QKeySequence::SelectAll );
    editMenu->addAction(tr("Select none"), sourceFrame, SLOT(selectNone()),
                        QKeySequence("Ctrl+0") );
    editMenu->addSeparator();
    editMenu->addAction(tr("Prettify"), sourceFrame, SLOT(prettify()) );
    editMenu->addSeparator();
    editMenu->addAction(tr("Edit settings"), this, SLOT(editSettings()) );

    moveMenu = menuBar()->addMenu(tr("&Move"));
    moveMenu->addAction(tr("Page forward"), sourceFrame, SLOT(pageForward()),
                        QKeySequence::MoveToNextPage );
    moveMenu->addAction(tr("Page backward"), sourceFrame, SLOT(pageBackward()),
                        QKeySequence::MoveToPreviousPage );
    moveMenu->addAction(tr("Go to line 1"), sourceFrame, SLOT(gotoFirstLine()),
                        QKeySequence::MoveToStartOfDocument );
    moveMenu->addAction(tr("Go to last line"), sourceFrame, SLOT(gotoLastLine()),
                        QKeySequence::MoveToEndOfDocument );
    moveMenu->addAction(tr("Go to line n"), sourceFrame, SLOT(gotoLine()),
                        QKeySequence("Ctrl+L") );
    moveMenu->addAction(tr("Go to top of screen"), sourceFrame, SLOT(gotoTop()),
                        QKeySequence("Ctrl+T") );
    moveMenu->addAction(tr("Go to bottom"), sourceFrame, SLOT(gotoBottom()),
                        QKeySequence("Ctrl+B") );
    moveMenu->addAction(tr("Move line to middle"), sourceFrame, SLOT(center()),
                        QKeySequence("Ctrl+M") );

    editToolBar->addAction(QIcon(QString(":/icons/%1/cut.png").arg(icon_size)),
        tr("Cut"), sourceFrame, SLOT(cut()) );
    editToolBar->addAction(QIcon(QString(":/icons/%1/copy.png").arg(icon_size)),
        tr("Copy"), sourceFrame, SLOT(copy()) );
    editToolBar->addAction(QIcon(QString(":/icons/%1/paste.png")
        .arg(icon_size)), tr("Paste"), sourceFrame, SLOT(paste()) );
    editToolBar->addSeparator();
    editToolBar->addAction(QIcon(QString(":/icons/%1/undo.png")
        .arg(icon_size)), tr("Undo"), sourceFrame, SLOT(undo()) );
    editToolBar->addAction(QIcon(QString(":/icons/%1/redo.png")
        .arg(icon_size)), tr("Redo"), sourceFrame, SLOT(redo()) );
    editToolBar->addSeparator();
    editToolBar->addAction(QIcon(QString(":/icons/%1/indent-more.png")
        .arg(icon_size)), tr("Indent"), sourceFrame, SLOT(indent()) );
    editToolBar->addAction(QIcon(QString(":/icons/%1/indent-less.png")
        .arg(icon_size)), tr("Unindent"), sourceFrame, SLOT(unIndent()) );
    editToolBar->addSeparator();
    editToolBar->addAction(QIcon(QString(":/icons/%1/find.png")
        .arg(icon_size)), tr("Find"), sourceFrame, SLOT(find()) );
    editToolBar->addAction(QIcon(QString(":/icons/%1/select-all.png")
        .arg(icon_size)), tr("Select all"), sourceFrame, SLOT(selectAll()) );
    editToolBar->addAction(QIcon(QString(":/icons/%1/prettify.png")
        .arg(icon_size)), tr("Prettify"), sourceFrame, SLOT(prettify()) );
    editToolBar->addSeparator();
    editToolBar->addAction(QIcon(QString(":/icons/%1/font-decrease.png")
        .arg(icon_size)), tr("Decrease font"), this, SLOT(decreaseFont()) );
    editToolBar->addAction(QIcon(QString(":/icons/%1/font-increase.png")
        .arg(icon_size)), tr("Increase font"), this, SLOT(increaseFont()) );

    addToolBar(Qt::TopToolBarArea,editToolBar);

    debugToolBar->addAction(QIcon(QString(":/icons/%1/ebe.png")
        .arg(icon_size)), tr("Run (F5)"), sourceFrame, SLOT(run()) );
    debugToolBar->actions()[0]->setShortcut(QKeySequence("F5"));
    debugToolBar->addAction(QIcon(QString(":/icons/%1/next.png")
        .arg(icon_size)), tr("Next (F6)"), sourceFrame, SLOT(next()) );
    debugToolBar->actions()[1]->setShortcut(QKeySequence("F6"));
    debugToolBar->addAction(QIcon(QString(":/icons/%1/step.png")
        .arg(icon_size)), tr("Step (F7)"), sourceFrame, SLOT(step()) );
    debugToolBar->actions()[2]->setShortcut(QKeySequence("F7"));
    debugToolBar->addAction(QIcon(QString(":/icons/%1/continue.png")
        .arg(icon_size)), tr("Continue (F8)"), sourceFrame, SLOT(Continue()) );
    debugToolBar->actions()[3]->setShortcut(QKeySequence("F8"));
    debugToolBar->addAction(QIcon(QString(":/icons/%1/process-stop.png")
        .arg(icon_size)), tr("stop (F9)"), sourceFrame, SLOT(stop()) );
    debugToolBar->actions()[4]->setShortcut(QKeySequence("F9"));

    addToolBar(Qt::TopToolBarArea,debugToolBar);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction ( dataDock->toggleViewAction() );
    viewMenu->addAction ( registerDock->toggleViewAction() );
    viewMenu->addAction ( floatDock->toggleViewAction() );
    viewMenu->addAction ( backTraceDock->toggleViewAction() );
    //viewMenu->addAction ( consoleDock->toggleViewAction() );
    viewMenu->addAction ( terminalDock->toggleViewAction() );
    viewMenu->addAction ( projectDock->toggleViewAction() );
    viewMenu->addAction ( toyBoxDock->toggleViewAction() );
    viewMenu->addAction ( bitBucketDock->toggleViewAction() );
    addToggle ( viewMenu, "Tooltips", this, SLOT(setTooltipsVisible(bool)),
                          ebe["tooltips/visible"].toBool() );
    addToggle ( viewMenu, "Command line", sourceFrame,
                          SLOT(setCommandLineVisible(bool)),
                          ebe["command/visible"].toBool() );


    fontMenu = menuBar()->addMenu(tr("F&ont"));
    fontMenu->addAction(tr("Increase font"), this, SLOT(increaseFont()),
                        QKeySequence::ZoomIn );
    fontMenu->addAction(tr("Decrease font"), this, SLOT(decreaseFont()),
                        QKeySequence::ZoomOut );

    helpMenu = menuBar()->addMenu(tr("&Help "));
    helpAction ( helpMenu, "Getting started", "start.html" );
    helpAction ( helpMenu, "Projects", "projects.html" );
    helpAction ( helpMenu, "Editing", "editing.html" );
    helpAction ( helpMenu, "Breakpoints", "breakpoints.html" );
    helpAction ( helpMenu, "Running", "running.html" );
    helpAction ( helpMenu, "About", "about.html" );

    templateToolBar->addAction(QIcon(QString(":/icons/%1/cin.png")
        .arg(icon_size)), "cin: input" );
    templateToolBar->addAction(QIcon(QString(":/icons/%1/cout.png")
        .arg(icon_size)), "cout: output");
    templateToolBar->addSeparator();
    templateToolBar->addAction(QIcon(QString(":/icons/%1/if.png")
        .arg(icon_size)), "if");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/if-else.png")
        .arg(icon_size)), "if-else");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/switch.png")
        .arg(icon_size)), "switch: multi-way branch");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/for.png")
        .arg(icon_size)), "for: counting loop");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/while.png")
        .arg(icon_size)), "while: general loop with test at the top");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/do-while.png")
        .arg(icon_size)), "do-while: general loop with test at the bottom");
    templateToolBar->addSeparator();
    templateToolBar->addAction(QIcon(QString(":/icons/%1/double.png")
        .arg(icon_size)), "double: floating point number");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/int.png")
        .arg(icon_size)), "int: an integer");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/string.png")
        .arg(icon_size)), "string");
    templateToolBar->addSeparator();
    templateToolBar->addAction(QIcon(QString(":/icons/%1/main.png")
        .arg(icon_size)), "main");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/function.png")
        .arg(icon_size)), "function");
    templateToolBar->addSeparator();
    templateToolBar->addAction(QIcon(QString(":/icons/%1/class.png")
        .arg(icon_size)), "class");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/struct.png")
        .arg(icon_size)), "struct");
    templateToolBar->addAction(QIcon(QString(":/icons/%1/library.png")
        .arg(icon_size)), "library");

    connect ( templateToolBar, SIGNAL(actionTriggered(QAction*)),
        sourceFrame, SLOT(doTemplate(QAction*)) );
    addToolBar(Qt::RightToolBarArea,templateToolBar);
}

void MainWindow::open(QString name)
{
    if ( name.endsWith(".ebe", Qt::CaseInsensitive) ) {
        projectWindow->open(name);
    } else {
        sourceFrame->open(name,0);
    }
}

void MainWindow::setTooltipsVisible(bool visible)
{
    ebe["tooltips/visible"] = visible;
    tooltipsVisible = visible;
}

QAction *MainWindow::addToggle ( QMenu *menu, QString text, QObject *object,
                             const char *slot, bool checked )
{
    QAction *action = new QAction ( text, this );
    action->setCheckable(true);
    action->setChecked(checked);
    menu->addAction(action);
    connect ( action, SIGNAL(triggered(bool)), object, slot );
    return action;
}


void MainWindow::helpAction ( QMenu *menu, QString text, QString file )
{
    QAction *action = new QAction( text, this );
    action->setData(file);
    menu->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(displayHelp()));
}

void MainWindow::displayHelp()
{
    QAction *action = (QAction *)sender();
    QWebView *view = new QWebView;
    view->load(QUrl("qrc:/html/"+action->data().toString()));
    view->setZoomFactor((double)fontSize/14);
    view->show();
}

void MainWindow::quit()
{
    if ( sourceFrame->filesSaved() ) {
#ifdef Q_WS_WIN
        if ( needToKill ) TerminateProcess(hProcess,0);
#else
        if ( gdbWaiting ) {
            kill ( gdbProcess->pid(), 2 );
        }
#endif
        emit doStop();
        saveSettings();
        qApp->quit();
    }
}

void MainWindow::changeDirectory()
{
    QString dir;

    dir = QFileDialog::getExistingDirectory(this,tr("Select working directory"),
                QDir::currentPath(), QFileDialog::ShowDirsOnly );
    if ( dir != "" ) {
        //qDebug() << "cd" << dir;
        QDir::setCurrent(dir);
        //qDebug() << "current" << QDir::current();
        settings->read();
        restoreMainWindow();
        emit sendWorkingDir(dir);
    }
}

void MainWindow::createStatusBar()
{
    ::statusBar = statusBar();
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    dataDock = new QDockWidget(tr("Data"));
    dataDock->setObjectName("Dock 1");
    dataDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    dataWindow = new DataWindow(dataDock);
    dataWindow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    dataDock->setWidget(dataWindow);
    addDockWidget(Qt::LeftDockWidgetArea, dataDock);

    registerDock = new QDockWidget(tr("Registers"));
    registerDock->setObjectName("Dock 2");
    registerDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    registerWindow = new RegisterWindow(this);    
    registerWindow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    registerDock->setWidget(registerWindow);
    addDockWidget(Qt::LeftDockWidgetArea, registerDock);

    floatDock = new QDockWidget(tr("Floating Point Registers"));
    floatDock->setObjectName("Dock 3");
    floatDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    floatWindow = new FloatWindow(floatDock);
    floatWindow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    addDockWidget(Qt::LeftDockWidgetArea, floatDock);
    floatDock->setWidget(floatWindow);

    projectDock = new QDockWidget(tr("Project"));
    projectDock->setObjectName("Dock 4");
    projectDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    projectWindow = new ProjectWindow(projectDock);
    projectWindow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    projectDock->setWidget(projectWindow);
    addDockWidget(Qt::LeftDockWidgetArea, projectDock);

    backTraceDock = new QDockWidget(tr("Back Trace"));
    backTraceDock->setObjectName("Dock 6");
    backTraceDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    backTraceWindow = new BackTraceWindow(backTraceDock);
    backTraceWindow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    backTraceDock->setWidget(backTraceWindow);
    addDockWidget(Qt::LeftDockWidgetArea, backTraceDock);

    terminalDock = new QDockWidget(tr("Terminal"));
    terminalDock->setObjectName("Dock 6");
    terminalDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    terminalWindow = new TerminalWindow(terminalDock);
    terminalDock->setWidget(terminalWindow);
    addDockWidget(Qt::LeftDockWidgetArea, terminalDock);

    toyBoxDock = new QDockWidget(tr("Toy Box"));
    toyBoxDock->setObjectName("Dock 7");
    toyBoxDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    toyBox = new ToyBox(toyBoxDock);
    toyBoxDock->setWidget(toyBox);
    addDockWidget(Qt::LeftDockWidgetArea, toyBoxDock);

    bitBucketDock = new QDockWidget(tr("Bit Bucket"));
    bitBucketDock->setObjectName("Dock 8");
    bitBucketDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    bitBucket = new BitBucket;
    bitBucketDock->setWidget(bitBucket);
    addDockWidget(Qt::LeftDockWidgetArea, bitBucketDock);

    //consoleDock = new QDockWidget(tr("Console"));
    //consoleDock->setObjectName("Dock 9");
    //consoleDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              //Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    //consoleWindow = new ConsoleWindow(consoleDock);
    //consoleDock->setWidget(consoleWindow);
    //addDockWidget(Qt::LeftDockWidgetArea, consoleDock);

    dataDock->setVisible(ebe["data/visible"].toBool());
    registerDock->setVisible(ebe["register/visible"].toBool());
    floatDock->setVisible(ebe["float/visible"].toBool());
    projectDock->setVisible(ebe["project/visible"].toBool());
    backTraceDock->setVisible(ebe["backtrace/visible"].toBool());
    terminalDock->setVisible(ebe["terminal/visible"].toBool());
    if ( userSetGeometry && userHeight < 768 ) ebe["toybox/visible"] = false;
    toyBoxDock->setVisible(ebe["toybox/visible"].toBool());
    if ( userSetGeometry && userHeight < 768 ) ebe["bitbucket/visible"] = false;
    bitBucketDock->setVisible(ebe["bitbucket/visible"].toBool());
    //consoleDock->setVisible(ebe["console/visible"].toBool());
}

void MainWindow::editSettings()
{
    SettingsDialog *dialog = new SettingsDialog();
    if ( dialog->exec() ) restoreMainWindow();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //printf("key %x\n",event->key());
    if ( event->matches(QKeySequence::ZoomIn) ) {
        increaseFont();
    } else if ( event->matches(QKeySequence::ZoomOut) ) {
        decreaseFont();
    }
    event->ignore();
}
