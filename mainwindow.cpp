#include <QtGui>
#include <QTimer>
#include <QPushButton>
#include <QtWebKit>
#include <QWebView>
#include <QApplication>
#include <QKeySequence>
#include <QMessageBox>
#include <unistd.h>

#include "mainwindow.h"
#include "errorwindow.h"
#include "sourcewindow.h"
#include "settings.h"
#include "stylesheet.h"

DataWindow *dataWindow;
SourceFrame *sourceFrame;
RegisterWindow *registerWindow;
FloatWindow *floatWindow;
ProjectWindow *projectWindow;
TerminalWindow *terminalWindow;
ConsoleWindow *consoleWindow;
BackTraceWindow *backTraceWindow;
GDB *gdb;
GDBThread *gdbThread;

QStatusBar *statusBar;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    settings = new Settings;
    settings->read();

    qRegisterMetaType<QList<IntSet> >("QList<IntSet>");
    qRegisterMetaType<QMap<QString,QString> >("QMap<QString,QString>");
    qRegisterMetaType<QMap<QString,ClassDefinition> >("QMap<QString,ClassDefinition>");
    qApp->installEventFilter(this);
    
    gdbThread = new GDBThread();
    gdbThread->start();
    while ( !gdb ) usleep(10);
    //qDebug() << "gdb" << gdb;

    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("ebe"));
    sourceFrame = new SourceFrame(this);
    setCentralWidget(sourceFrame);

    createMenus();


    setUnifiedTitleAndToolBarOnMac(true);

    QTimer::singleShot(0,this,SLOT(restoreMainWindow()));

    connect ( gdb, SIGNAL(sendRegs(QMap<QString,QString>)),
              registerWindow, SLOT(receiveRegs(QMap<QString,QString>)) );
    connect ( gdb, SIGNAL(sendFpRegs(QStringList)),
              floatWindow, SLOT(receiveFpRegs(QStringList)) );
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
    consoleDock->setFloating(ebe["console/floating"].toBool());

    if ( ebe.contains("ebe/geometry") ) {
        restoreGeometry(ebe["ebe/geometry"].toByteArray());
    } else {
        resize(1000,800);
    }
    if ( ebe.contains("ebe/state") ) {
        restoreState(ebe["ebe/state"].toByteArray());
    } else {
        resize(1000,800);
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
    consoleDock->setStyleSheet("QDockWidget::title { font-family: " +
                            ebe["variable_font"].toString() + "}" );

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
    ebe["console/floating"]  = consoleDock->isFloating();
    ebe["data/visible"]      = dataDock->isVisible();
    ebe["register/visible"]  = registerDock->isVisible();
    ebe["float/visible"]     = floatDock->isVisible();
    ebe["project/visible"]   = projectDock->isVisible();
    ebe["terminal/visible"]  = terminalDock->isVisible();
    ebe["console/visible"]   = consoleDock->isVisible();
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
    fileMenu->addSeparator();
    fileMenu->addAction(tr("New project"), projectWindow, SLOT(newProject()) );
    fileMenu->addAction(tr("Open project"), projectWindow, SLOT(openProject()) );
    fileMenu->addAction(tr("Close project"), projectWindow, SLOT(closeProject()) );
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Quit"), this, SLOT(quit()), QKeySequence::Quit );

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

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction ( dataDock->toggleViewAction() );
    viewMenu->addAction ( registerDock->toggleViewAction() );
    viewMenu->addAction ( floatDock->toggleViewAction() );
    viewMenu->addAction ( backTraceDock->toggleViewAction() );
    viewMenu->addAction ( consoleDock->toggleViewAction() );
    viewMenu->addAction ( terminalDock->toggleViewAction() );
    viewMenu->addAction ( projectDock->toggleViewAction() );
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

}

void MainWindow::open(QString name)
{
    if ( name.endsWith(".ebe", Qt::CaseInsensitive) ) {
        projectWindow->open(name);\
    } else {
        sourceFrame->open(name);
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
    QWebView *view = new QWebView();
    view->load(QUrl("qrc:/html/"+action->data().toString()));
    view->show();
}

void MainWindow::quit()
{
    if ( sourceFrame->filesSaved() ) {
        saveSettings();
        qApp->quit();
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
    addDockWidget(Qt::BottomDockWidgetArea, terminalDock);

    consoleDock = new QDockWidget(tr("Console"));
    consoleDock->setObjectName("Dock 7");
    consoleDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    consoleWindow = new ConsoleWindow(consoleDock);
    consoleDock->setWidget(consoleWindow);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

    dataDock->setVisible(ebe["data/visible"].toBool());
    registerDock->setVisible(ebe["register/visible"].toBool());
    floatDock->setVisible(ebe["float/visible"].toBool());
    projectDock->setVisible(ebe["project/visible"].toBool());
    backTraceDock->setVisible(ebe["backtrace/visible"].toBool());
    terminalDock->setVisible(ebe["terminal/visible"].toBool());
    consoleDock->setVisible(ebe["console/visible"].toBool());
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
