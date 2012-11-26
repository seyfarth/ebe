#include <QtGui>
#include <QPushButton>
#include <QtWebKit>
#include <QWebView>
#include <QApplication>
#include <QKeySequence>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    source = new SourceWindow(this);
    setCentralWidget(source);

    createMenus();
    
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("ebe"));

    setUnifiedTitleAndToolBarOnMac(true);

    fontSize = 15;
    increaseFont();
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
    fileMenu->addAction(tr("New"), source, SLOT(newFile()), QKeySequence::New );
    templateMenu = fileMenu->addMenu(tr("&Template"));
        templateMenu->addAction(tr("C Program"), source, SLOT(templateC()));
        templateMenu->addAction(tr("C++ Program"), source, SLOT(templateCpp()));
        templateMenu->addAction(tr("Assembly Program"), source,
                      SLOT(templateAssembly()));
        templateMenu->addAction(tr("Fortran Program"), source, SLOT(templateFortran()));
    fileMenu->addAction(tr("Open"), source, SLOT(open()), QKeySequence::Open );
    fileMenu->addAction(tr("Save"), source, SLOT(save()), QKeySequence::Save );
    fileMenu->addAction(tr("Save as"), source, SLOT(saveAs()) );
    fileMenu->addSeparator();
    fileMenu->addAction(tr("New project"), source, SLOT(newProject()) );
    fileMenu->addAction(tr("Open project"), source, SLOT(openProject()) );
    fileMenu->addAction(tr("Close project"), source, SLOT(closeProject()) );
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Save settings"), source, SLOT(saveSettingsAs()) );
    fileMenu->addAction(tr("Save settings as"), source, SLOT(saveSettingsAs()) );
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Quit"), source, SLOT(quit()), QKeySequence::Quit );

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(tr("Cut"), source, SLOT(cut()), QKeySequence::Cut );
    editMenu->addAction(tr("Copy"), source, SLOT(copy()), QKeySequence::Copy );
    editMenu->addAction(tr("Paste"), source, SLOT(paste()), QKeySequence::Paste );
    editMenu->addSeparator();
    editMenu->addAction(tr("Undo"), source, SLOT(undo()), QKeySequence::Undo );
    editMenu->addAction(tr("Redo"), source, SLOT(redo()), QKeySequence::Redo );
    editMenu->addSeparator();
    editMenu->addAction(tr("Comment"), source, SLOT(Comment()), "Ctrl+K" );
    editMenu->addAction(tr("Uncomment"), source, SLOT(unComment()), "Ctrl+U"  );
    editMenu->addAction(tr("indent"), source, SLOT(indent()), "Ctrl+>"  );
    editMenu->addAction(tr("Unindent"), source, SLOT(unIndent()), "Ctrl+<"  );
    editMenu->addSeparator();
    editMenu->addAction(tr("Find"), source, SLOT(find()), QKeySequence::Find );
    editMenu->addSeparator();
    editMenu->addAction(tr("Select all"), source, SLOT(selectAll()),
                        QKeySequence::SelectAll );
    editMenu->addAction(tr("Select none"), source, SLOT(selectNone()), "Ctrl+0" );
    editMenu->addSeparator();
    editMenu->addAction(tr("Prettify"), source, SLOT(Prettify()) );

    moveMenu = menuBar()->addMenu(tr("&Move"));
    moveMenu->addAction(tr("Page forward"), source, SLOT(pageForward()),
                        QKeySequence::MoveToNextPage );
    moveMenu->addAction(tr("Page backward"), source, SLOT(pageBackward()),s
                        QKeySequence::MoveToPreviousPage );
    moveMenu->addAction(tr("Go to line 1"), source, SLOT(gotoFirstLine()),
                        QKeySequence::MoveToStartOfDocument );
    moveMenu->addAction(tr("Go to last line"), source, SLOT(gotoLastLine()),
                        QKeySequence::MoveToEndOfDocument );
    moveMenu->addAction(tr("Go to line n"), source, SLOT(gotoLine()), "Ctrl+L" );
    moveMenu->addAction(tr("Cut"), source, SLOT(cut()), QKeySequence::Cut );
    moveMenu->addAction(gotoTopAction);
    moveMenu->addAction(tr("Cut"), source, SLOT(cut()), QKeySequence::Cut );
    moveMenu->addAction(gotoBottomAction);
    moveMenu->addAction(tr("Cut"), source, SLOT(cut()), QKeySequence::Cut );
    moveMenu->addAction(centerAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    addToggle ( viewMenu, "Data window", this, SLOT(setDataVisible(bool)) );
    addToggle ( viewMenu, "Register window", this, SLOT(setRegistersVisible(bool)) );
    addToggle ( viewMenu, "Float register window", this, SLOT(setFloatsVisible(bool)) );
    addToggle ( viewMenu, "Console window", this, SLOT(setConsoleVisible(bool)) );
    addToggle ( viewMenu, "Project window", this, SLOT(setProjectVisible(bool)) );
    addToggle ( viewMenu, "Tooltips", this, SLOT(setTooltipsVisible(bool)) );
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
