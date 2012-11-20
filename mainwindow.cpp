#include <QtGui>
#include <QPushButton>
#include <QtWebKit>
#include <QWebView>
#include <QApplication>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    source = new SourceWindow(this);
    setCentralWidget(source);

    createActions();
    createMenus();
    
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("ebe"));

    setUnifiedTitleAndToolBarOnMac(true);

    fontSize = 15;
    increaseFont();
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New \tCtrl+n"),this);
    openAction = new QAction(tr("&Open \tCtrl+o"),this);
    saveAction = new QAction(tr("&Save \tCtrl+s"),this);
    saveAsAction = new QAction(tr("Save As"),this);
    closeAction = new QAction(tr("&Close"),this);
    newProjectAction = new QAction(tr("New project"),this);
    openProjectAction = new QAction(tr("Open Project"),this);
    closeProjectAction = new QAction(tr("Close Project"),this);
    saveSettingsAction = new QAction(tr("Save settings"),this);
    saveSettingsAsAction = new QAction(tr("Save settings as"),this);
    quitAction = new QAction(tr("&Quit \tCtrl+q"),this);
    templateCAction = new QAction(tr("New C Program"),this);
    templateCppAction = new QAction(tr("New C++ Program"),this);
    templateAssemblyAction = new QAction(tr("New Assembly Program"),this);
    templateFortranAction = new QAction(tr("New Fortran Program"),this);

    cutAction = new QAction(tr("Cut \tCtrl+x"),this);
    copyAction = new QAction(tr("Copy \tCtrl+c"),this);
    pasteAction = new QAction(tr("Paste \tCtrl+v"),this);
    undoAction = new QAction(tr("Undo \tCtrl+z"),this);
    redoAction = new QAction(tr("Redo \tCtrl+r"),this);
    commentAction = new QAction(tr("Comment \tCtrl+k"),this);
    uncommentAction = new QAction(tr("Uncomment \tCtrl+u"),this);
    indentAction = new QAction(tr("Indent \tCtrl+>"),this);
    unindentAction = new QAction(tr("Unindent \tCtrl+<"),this);
    findAction = new QAction(tr("Find \tCtrl+f"),this);
    selectAllAction = new QAction(tr("Select all \tCtrl+a"),this);
    selectNoneAction = new QAction(tr("Select none \tCtrl+0"),this);
    prettifyAction = new QAction(tr("Prettify \tCtrl+p"),this);

    pageFwdAction = new QAction(tr("Page forward \tPageDown"),this);
    pageBackAction = new QAction(tr("Page back \tPageUp"),this);
    firstLineAction = new QAction(tr("Goto line 1 \tCtrl+Home"),this);
    lastLineAction = new QAction(tr("Goto last line \tCtrl+End"),this);
    gotoLineAction = new QAction(tr("Goto line n \tCtrl+g"),this);
    gotoTopAction = new QAction(tr("Goto top of screen \tCtrl+t"),this);
    gotoBottomAction = new QAction(tr("Goto bottom of screen \tCtrl+b"),this);
    centerAction = new QAction(tr("Center line on screen \tCtrl+m"),this);

    dataAction = new QAction(tr("Data"),this);
    registerAction = new QAction(tr("Register"),this);
    floatAction = new QAction(tr("Float"),this);
    consoleAction = new QAction(tr("Console"),this);
    projectAction = new QAction(tr("Project"),this);
    tooltipsAction = new QAction(tr("Tooltips"),this);

    increaseAction = new QAction(tr("Increase \tCtrl +"),this);
    decreaseAction = new QAction(tr("Decrease \tCtrl -"),this);
}

void MainWindow::increaseFont()
{
    char style[40];

    fontSize++;
    sprintf(style,"* {font-size: %dpx}",fontSize);
    qApp->setStyleSheet(style);
}

void MainWindow::decreaseFont()
{
    char style[40];

    fontSize--;
    sprintf(style,"* {font-size: %dpx}",fontSize);
    qApp->setStyleSheet(style);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    templateMenu = fileMenu->addMenu(tr("&Template"));
        templateMenu->addAction(templateCAction);
        templateMenu->addAction(templateCppAction);
        templateMenu->addAction(templateAssemblyAction);
        templateMenu->addAction(templateFortranAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(newProjectAction);
    fileMenu->addAction(openProjectAction);
    fileMenu->addAction(closeProjectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveSettingsAction);
    fileMenu->addAction(saveSettingsAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(commentAction);
    editMenu->addAction(uncommentAction);
    editMenu->addAction(indentAction);
    editMenu->addAction(unindentAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAction);
    editMenu->addAction(selectNoneAction);
    editMenu->addSeparator();
    editMenu->addAction(prettifyAction);

    moveMenu = menuBar()->addMenu(tr("&Move"));
    moveMenu->addAction(pageFwdAction);
    moveMenu->addAction(pageBackAction);
    moveMenu->addAction(firstLineAction);
    moveMenu->addAction(lastLineAction);
    moveMenu->addAction(gotoLineAction);
    moveMenu->addAction(gotoTopAction);
    moveMenu->addAction(gotoBottomAction);
    moveMenu->addAction(centerAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(dataAction);
    viewMenu->addAction(registerAction);
    viewMenu->addAction(floatAction);
    viewMenu->addAction(consoleAction);
    viewMenu->addAction(projectAction);
    viewMenu->addAction(tooltipsAction);
    addToggle ( viewMenu, "Command line", source, SLOT(setCommandLineVisible(bool)) );

    fontMenu = menuBar()->addMenu(tr("F&ont"));
    fontMenu->addAction(increaseAction);
    fontMenu->addAction(decreaseAction);
    connect(increaseAction,SIGNAL(triggered()),this,SLOT(increaseFont()));
    connect(decreaseAction,SIGNAL(triggered()),this,SLOT(decreaseFont()));

    helpMenu = menuBar()->addMenu(tr("&Help "));
    helpAction ( helpMenu, "Getting started", "start.html" );
    helpAction ( helpMenu, "Projects", "projects.html" );
    helpAction ( helpMenu, "Editing", "editing.html" );
    helpAction ( helpMenu, "Breakpoints", "breakpoints.html" );
    helpAction ( helpMenu, "Running", "running.html" );
    helpAction ( helpMenu, "About", "about.html" );

}

void MainWindow::addToggle ( QMenu *menu, QString text, QObject *object, const char *slot )
{
    QAction *action = new QAction ( text, this );
    action->setCheckable(true);
    action->setChecked(true);
    menu->addAction(action);
    connect ( action, SIGNAL(triggered(bool)), object, slot );
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
    qDebug() << "Calling static QApp quit()";

    // FIXME: save project/files etc., before quitting
    QApplication::quit();
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Data"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    QPushButton *button = new QPushButton(tr("hello"),dock);
    button->setMinimumHeight(0);
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    dock->setWidget(button);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Registers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    button = new QPushButton(tr("hello"),dock);
    button->setMinimumHeight(0);
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    dock->setWidget(button);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Floating Point Registers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    button = new QPushButton(tr("hello"),dock);
    button->setMinimumHeight(0);
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    dock->setWidget(button);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Project"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    button = new QPushButton(tr("hello"),dock);
    button->setMinimumHeight(0);
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    dock->setWidget(button);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Terminal"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    button = new QPushButton(tr("hello"),dock);
    button->setMinimumHeight(0);
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    dock->setWidget(button);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Console"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    button = new QPushButton(tr("hello"),dock);
    button->setMinimumHeight(0);
    button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Ignored);
    dock->setWidget(button);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ( event->matches(QKeySequence::ZoomIn) ) {
        increaseFont();
    } else if ( event->matches(QKeySequence::ZoomOut) ) {
        decreaseFont();
    }
    event->ignore();
}
