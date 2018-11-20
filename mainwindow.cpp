#include <QtGui>
#include <QtGlobal>
#include <QDesktopWidget>
#include <QTimer>
#include <QTextBrowser>
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>
#include <QDockWidget>
#include <QToolBar>
#include <QMenuBar>
#include <QFileDialog>
#ifdef Q_OS_WIN32
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
extern int userWidth;
extern int userHeight;
extern Settings *settings;
extern QApplication *app;

QString gdbName;
DataWindow *dataWindow;
SourceFrame *sourceFrame;
RegisterWindow *registerWindow;
FrameWindow *frameWindow;
AsmDataWindow *asmDataWindow;
HalRegisterWindow *halRegisterWindow;
HalNamesWindow *halNamesWindow;
FloatWindow *floatWindow;
ProjectWindow *projectWindow;
TerminalWindow *terminalWindow;
ConsoleWindow *consoleWindow;
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

#ifdef Q_OS_WIN32
extern HANDLE hProcess;
extern bool needToKill;
#else
extern int gdbWaiting;
#endif

void MainWindow::setWordSize()
{
#ifdef Q_OS_WIN32
    QProcess where(app);
    where.start("where objdump.exe");
    where.waitForFinished();
    QByteArray data = where.readLine();
    QString fileName;
    fileName = QString(data.trimmed());
    //qDebug() << "file" << fileName;
    where.start(QString("objdump.exe -f \"%1\"").arg(fileName));
    where.waitForFinished();
    data = where.readAllStandardOutput();
    //qDebug() << data;
    if ( QString(data).indexOf("x86-64") >= 0 ) {
        wordSize = 64;
    } else {
        wordSize = 32;
    }
#endif
#ifdef Q_OS_LINUX
    QProcess which(app);
    which.start("which objdump");
    which.waitForFinished();
    QByteArray data = which.readLine();
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
#if defined(Q_OS_BSD4) && !defined(Q_OS_MAC)
    QProcess which(app);
    which.start("which objdump");
    which.waitForFinished();
    QByteArray data = which.readLine();
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
    QProcess which(app);
    which.start("which nm");
    which.waitForFinished();
    QByteArray data = which.readLine();
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
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    preferredWindowSize()
{
    qRegisterMetaType < QList<StringSet> > ("QList<StringSet>");
    qRegisterMetaType < StringHash > ("StringHash");
    qRegisterMetaType < QHash<QString, ClassDefinition>
        > ("QHash<QString,ClassDefinition>");
    qApp->installEventFilter(this);

    checkTools();

    gdbThread = new GDBThread();
    gdbThread->start();

#ifdef Q_OS_WIN32
    int sleepTime = 1;
    while ( !gdb ) {
        Sleep(sleepTime);
        if ( sleepTime > 10 ) qDebug() << "gdb is taking too long";
        sleepTime++;
    }
#else
    int sleepTime = 10000;
    while (!gdb) {
        usleep(sleepTime);
        if ( sleepTime > 1000000 ) qDebug() << "gdb is taking too long";
        sleepTime += 1000000;
    }
#endif

    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("ebe"));
    sourceFrame = new SourceFrame(this);
    setCentralWidget(sourceFrame);

    createMenus();

    readInstructions();

    setUnifiedTitleAndToolBarOnMac(false);

    QTimer::singleShot(0, this, SLOT(restoreMainWindow()));

    connect(gdb, SIGNAL(sendRegs(StringHash)), registerWindow,
        SLOT(receiveRegs(StringHash)));
    if ( wordSize == 64 ) {
        connect(gdb, SIGNAL(sendRegs(StringHash)), halRegisterWindow,
            SLOT(receiveRegs(StringHash)));
    }
    connect(gdb, SIGNAL(sendFpRegs(QStringList)), floatWindow,
        SLOT(receiveFpRegs(QStringList)));
    connect(this, SIGNAL(sendWorkingDir(QString)), gdb,
        SLOT(receiveWorkingDir(QString)));
    connect(this, SIGNAL(doStop()), gdb, SLOT(doStop()));
    //dDebug() << "Connected";
    restoreMainWindow();
    //dDebug() << "Restored";
}

bool MainWindow::toolExists(QString tool)
{
    QString path;
    QStringList pathList;
    path = qgetenv("PATH").constData();
    if (ebe.os == "windows") {
        pathList = path.split(";");
        tool += ".exe";
    } else {
        pathList = path.split(":");
    }
    foreach (QString dir, pathList )
    {
        path = dir + "/" + tool;
        //qDebug() << "Check" << path;
        if ( QDir::current().exists(path) )
        {
            //qDebug() << path << "found";
            return true;
        }
        //qDebug() << path << "not found";
    }
    return false;
}

void MainWindow::checkTools()
{
    QStringList missing;
    QStringList missingCritical;
    if (ebe["check/tools"].toBool()) {
        if (toolExists("gdb")) {
            gdbName = "gdb";
        } else if ( toolExists("ggdb")) {
            gdbName = "ggdb";
        } else {
            missingCritical += "gdb";
        }
#if defined(Q_OS_WIN)
        if (!toolExists("nm")) {
            QString message;

            message = "<b>" +
                tr("It seems that ebetools needs to be installed") +
                "</b>";
            QMessageBox::warning(this, tr("Ebetools"), message,
                QMessageBox::Ok );
            exit(1);
        }
#endif
#if defined(Q_OS_BSD4)
        if (!toolExists("cc")) missingCritical += "cc";
#else
        if (!toolExists("gcc")) missingCritical += "gcc";
        //if (!toolExists("gfortran") && !toolExists("g95") ) {
            //missing += "gfortran";
        //}
#endif
        if (!toolExists("nasm")) missing += "nasm";
        if (!toolExists("astyle")) missing += "astyle";
        if (missing.length() > 0 || missingCritical.length() > 0) {
            //qDebug() << "oops";
            QString message;

            message = "<b>" + tr("Some tools used by ebe are missing:")
                + "</b>";
            message += "<br />";
            message += "<br />";
            foreach ( QString tool, missingCritical )
            {
                message += tr("   critical: ") + tool + "<br />";
            }
            foreach ( QString tool, missing )
            {
                message += tr("   non-critical: ") + tool + "<br />";
            }
            message += "<br />";
            message += tr("Critical tools are needed by almost everyone.");
            message += "<br />";
            message += tr("Non-critical tools are not needed by everyone.");
            message += "<br />";
            message += "<br />";
            message += tr(
                "Click <b>Ignore</b> to ignore this test in the future.");
            message += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
            message += "<br />";
            message += tr(
                "Click <b>OK</b> to run ebe and test again next time");
            message += "<br />";
            int ret = QMessageBox::warning(this, tr("Missing tools"), message,
                QMessageBox::Ok | QMessageBox::Ignore);
            if (ret == QMessageBox::Ignore) ebe["check/tools"] = false;
        }
    }
    if (toolExists("gdb")) {
        gdbName = "gdb";
    } else {
        gdbName = "ggdb";
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (sourceFrame->filesSaved()) {
#ifdef Q_OS_WIN32
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

    cursorFlashTime = ebe["cursor_flash"].toInt();
    //qDebug() << "Setting flash" << cursorFlashTime;
    app->setCursorFlashTime(cursorFlashTime);

    addStyleSheet("textedit-font",
        QString("QTextEdit { font-weight: bold; font-family: %1}").arg(
            ebe["fixed_font"].toString()));
    addStyleSheet("plaintextedit-font",
        QString("QPlainTextEdit { font-weight: bold; font-family: %1}").arg(
            ebe["fixed_font"].toString()));
    addStyleSheet("plaintextedit-bg",
        QString("QPlainTextEdit { background: %1}").arg(
            ebe["bg_color"].toString()));
    addStyleSheet("table-bg",
        QString("QTableWidget { background: %1}").arg(
            ebe["table_bg"].toString()));
    addStyleSheet("list-bg",
        QString("QListWidget { background: %1}").arg(
            ebe["list_bg"].toString()));
    addStyleSheet("tree-bg",
        QString("QTreeWidget { background: %1}").arg(
            ebe["tree_bg"].toString()));
    addStyleSheet("lineedit-font",
        QString("QLineEdit { font-weight: bold; font-family: %1}").arg(
            ebe["fixed_font"].toString()));
    addStyleSheet("table-font",
        QString("QTableView { font-weight: bold; font-family: %1}").arg(
            ebe["fixed_font"].toString()));
    addStyleSheet("list-font",
        QString("QListWidget { font-weight: bold; font-family: %1}").arg(
            ebe["fixed_font"].toString()));
    addStyleSheet("tree-font",
        QString("QTreeWidget { font-weight: bold; font-family: %1}").arg(
            ebe["fixed_font"].toString()));
    addStyleSheet("tab-font",
        QString("QTabBar { font-family: %1}").arg(
            ebe["variable_font"].toString()));
    //dDebug() << "add ss";
    dataDock->setFloating(ebe["data/floating"].toBool());
    registerDock->setFloating(ebe["register/floating"].toBool());
    if ( wordSize == 64 ) {
        halRegisterDock->setFloating(ebe["halregister/floating"].toBool());
        halNamesDock->setFloating(ebe["halnames/floating"].toBool());
    }
    frameDock->setFloating(ebe["frame/floating"].toBool());
    asmDataDock->setFloating(ebe["asmdata/floating"].toBool());
    floatDock->setFloating(ebe["float/floating"].toBool());
    projectDock->setFloating(ebe["project/floating"].toBool());
    terminalDock->setFloating(ebe["terminal/floating"].toBool());
    toyBoxDock->setFloating(ebe["toybox/floating"].toBool());
    bitBucketDock->setFloating(ebe["bitbucket/floating"].toBool());
    consoleDock->setFloating(ebe["console/floating"].toBool());

    //dDebug() << "setgeo";
    if (!userSetGeometry) {
        bool hasGeometry = ebe.contains("ebe/geometry");
        bool hasState = ebe.contains("ebe/state");

        if (hasGeometry) {
            restoreGeometry(ebe["ebe/geometry"].toByteArray());
        }
        if (hasState) {
            restoreState(ebe["ebe/state"].toByteArray());
        }

        if (!hasState && !hasGeometry) {
            resize(preferredWindowSize.width(), preferredWindowSize.height());
        }
    }
    else { // userSetGeometry
        resize(userWidth, userHeight);
    }

    dataDock->setStyleSheet(
        "QDockWidget::title { font-family: " + ebe["variable_font"].toString()
            + "}");
    registerDock->setStyleSheet(
        "QDockWidget::title { font-family: " + ebe["variable_font"].toString()
            + "}");
    floatDock->setStyleSheet(
        "QDockWidget::title { font-family: " + ebe["variable_font"].toString()
            + "}");
    projectDock->setStyleSheet(
        "QDockWidget::title { font-family: " + ebe["variable_font"].toString()
            + "}");
    terminalDock->setStyleSheet(
        "QDockWidget::title { font-family: " + ebe["variable_font"].toString()
            + "}");
    consoleDock->setStyleSheet(
        "QDockWidget::title { font-family: " + ebe["variable_font"].toString()
            + "}");

    //dDebug() << "set fs";
    fontSize = ebe["font_size"].toInt();
    setFontSize();
    //dDebug() << "set";
}

bool MainWindow::eventFilter(QObject * /* object */, QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        if (!tooltipsVisible) return true;
    }
    return false;
}

void MainWindow::saveSettings()
{
    ebe["ebe/geometry"] = saveGeometry();
    ebe["ebe/state"] = saveState();
    ebe["data/floating"] = dataDock->isFloating();
    ebe["register/floating"] = registerDock->isFloating();
    if ( wordSize == 64 ) {
        ebe["halregister/floating"] = halRegisterDock->isFloating();
        ebe["halnames/floating"] = halNamesDock->isFloating();
    }
    ebe["frame/floating"] = frameDock->isFloating();
    ebe["asmdata/floating"] = asmDataDock->isFloating();
    ebe["float/floating"] = floatDock->isFloating();
    ebe["project/floating"] = projectDock->isFloating();
    ebe["terminal/floating"] = terminalDock->isFloating();
    ebe["toybox/floating"] = toyBoxDock->isFloating();
    ebe["bitbucket/floating"] = bitBucketDock->isFloating();
    ebe["console/floating"] = consoleDock->isFloating();
    ebe["data/visible"] = dataDock->isVisible();
    ebe["register/visible"] = registerDock->isVisible();
    if ( wordSize == 64 ) {
        ebe["halregister/visible"] = halRegisterDock->isVisible();
        ebe["halnames/visible"] = halNamesDock->isVisible();
    } else {
        ebe["halregister/visible"] = false;
        ebe["halnames/visible"] = false;
    }
    ebe["frame/visible"] = frameDock->isVisible();
    ebe["asmdata/visible"] = asmDataDock->isVisible();
    ebe["float/visible"] = floatDock->isVisible();
    ebe["project/visible"] = projectDock->isVisible();
    ebe["terminal/visible"] = terminalDock->isVisible();
    ebe["toybox/visible"] = toyBoxDock->isVisible();
    ebe["bitbucket/visible"] = bitBucketDock->isVisible();
    ebe["console/visible"] = consoleDock->isVisible();
    ebe["cursor_flash"] = cursorFlashTime;
    settings->write();
}

void MainWindow::setFontSize()
{
    int width;
    int height;
    if (fontSize < 4) fontSize = 4;
    if (fontSize > 100) fontSize = 100;
    ebe["font_size"] = fontSize;
    addStyleSheet("font-size",
        "* {font-size: " + QString("%1").arg(fontSize) + "px}");
    //addStyleSheet("table-font-size", "QTableView {font-size: " + QString("%1").arg(fontSize) + "px}");
    QFont f("courier");
    f.setPixelSize(fontSize);
    f.setBold(true);
    QFontMetrics fm(f);
    width = fm.width(QChar('m'));
    height = fm.height();
    if ( width < height/2.0 ) width++;
    //qDebug() << "fs" << fontSize << width << height;
    sourceFrame->setFontHeightAndWidth(height, width);
    dataWindow->setFontHeightAndWidth(height, width);
    floatWindow->setFontHeightAndWidth(height, width);
    //dDebug() << "regw";
    registerWindow->setFontHeightAndWidth(height, width);
    if ( wordSize == 64 ) {
        halRegisterWindow->setFontHeightAndWidth(height, width);
        halNamesWindow->setFontHeightAndWidth(height, width);
    }
    frameWindow->setFontHeightAndWidth(height, width);
    asmDataWindow->setFontHeightAndWidth(height, width);
    //dDebug() << "hregw";
    toyBox->setFontHeightAndWidth(height, width);
    bitBucket->setFontHeightAndWidth(height, width);
    //dDebug() << "done sfs";
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

    app->setWindowIcon(QIcon(QString(":/icons/%1/ebe.png").arg(icon_size)));

    fileToolBar = new QToolBar(tr("File toolbar"), this);
    fileToolBar->setObjectName(tr("File toolbar"));
    fileToolBar->setIconSize(QSize(icon_size, icon_size));

    editToolBar = new QToolBar(tr("Edit toolbar"), this);
    editToolBar->setObjectName(tr("Edit toolbar"));
    editToolBar->setIconSize(QSize(icon_size, icon_size));

    debugToolBar = new QToolBar(tr("Debug toolbar"), this);
    debugToolBar->setObjectName(tr("Debug toolbar"));
    debugToolBar->setIconSize(QSize(icon_size, icon_size));

    templateToolBar = new QToolBar(tr("Code template toolbar"), this);
    templateToolBar->setObjectName(tr("Template toolbar"));
    templateToolBar->setIconSize(QSize(icon_size, icon_size));

    menuBar()->setStyleSheet("font-family: " + ebe["variable_font"].toString());
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&New"), sourceFrame, SLOT(newFile()),
        QKeySequence::New);
    templateMenu = fileMenu->addMenu(tr("&Template"));
    templateMenu->addAction(tr("&C Program"), sourceFrame, SLOT(templateC()));
    templateMenu->addAction(tr("C&++ Program"), sourceFrame,
        SLOT(templateCpp()));
    templateMenu->addAction(tr("&Assembly Program"), sourceFrame,
        SLOT(templateAssembly()));
    templateMenu->addAction(tr("&HAL Program"), sourceFrame,
        SLOT(templateHal()));
    //templateMenu->addAction(tr("Fortran Program"), sourceFrame,
        //SLOT(templateFortran()));
    fileMenu->addAction(tr("&Open"), sourceFrame, SLOT(open(bool)), QKeySequence::Open );
    fileMenu->addAction(tr("&Save"), sourceFrame, SLOT(save()),
        QKeySequence::Save);
    fileMenu->addAction(tr("Save &as"), sourceFrame, SLOT(saveAs()));
    fileMenu->addAction(tr("&Close"), sourceFrame, SLOT(close()));
    fileMenu->addAction(tr("Change &directory"), this, SLOT(changeDirectory()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("N&ew project"), projectWindow, SLOT(newProject()));
    fileMenu->addAction(tr("O&pen project"), projectWindow, SLOT(openProject()));
    fileMenu->addAction(tr("C&lose project"), projectWindow,
        SLOT(closeProject()));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Quit"), this, SLOT(quit()), QKeySequence::Quit);

    fileToolBar->addAction(QIcon(QString(":/icons/%1/quit.png").arg(icon_size)),
        tr("Quit (Ctrl+Q)"), this, SLOT(quit()));
    fileToolBar->addSeparator();
    fileToolBar->addAction(QIcon(QString(":/icons/%1/new.png").arg(icon_size)),
        tr("New (Ctrl+N)"), sourceFrame, SLOT(newFile()));
    fileToolBar->addAction(QIcon(QString(":/icons/%1/open.png").arg(icon_size)),
        tr("Open (Ctrl+O)"), sourceFrame, SLOT(open(bool)) );
    fileToolBar->addAction(QIcon(QString(":/icons/%1/save.png").arg(icon_size)),
        tr("Save (Ctrl+S)"), sourceFrame, SLOT(save()));
    fileToolBar->addAction(
        QIcon(QString(":/icons/%1/save-as.png").arg(icon_size)), tr("Save as"),
        sourceFrame, SLOT(saveAs()));
    fileToolBar->addAction(
        QIcon(QString(":/icons/%1/close.png").arg(icon_size)), tr("Close (Ctrl+C)"),
        sourceFrame, SLOT(close()));

    fileToolBar->addSeparator();
    fileToolBar->addAction(
        QIcon(QString(":/icons/%1/project-new.png").arg(icon_size)),
        tr("New project"), projectWindow, SLOT(newProject()));
    fileToolBar->addAction(
        QIcon(QString(":/icons/%1/project-open.png").arg(icon_size)),
        tr("Open project"), projectWindow, SLOT(openProject()));
    fileToolBar->addAction(
        QIcon(QString(":/icons/%1/project-close.png").arg(icon_size)),
        tr("Close project"), projectWindow, SLOT(closeProject()));

    addToolBar(Qt::TopToolBarArea, fileToolBar);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(tr("Cut &X"), sourceFrame, SLOT(cut()), QKeySequence::Cut);
    editMenu->addAction(tr("&Copy"), sourceFrame, SLOT(copy()),
        QKeySequence::Copy);
    editMenu->addAction(tr("Paste &V"), sourceFrame, SLOT(paste()),
        QKeySequence::Paste);
    editMenu->addSeparator();
    editMenu->addAction(tr("Undo &Z"), sourceFrame, SLOT(undo()),
        QKeySequence::Undo);
    editMenu->addAction(tr("&Redo"), sourceFrame, SLOT(redo()),
        QKeySequence::Redo);
    editMenu->addSeparator();
    editMenu->addAction(tr("Comment &K"), sourceFrame, SLOT(comment()),
        QKeySequence("Ctrl+K"));
    editMenu->addAction(tr("Uncomment &U"), sourceFrame, SLOT(unComment()),
        QKeySequence("Ctrl+U"));
    editMenu->addAction(tr("Indent &>"), sourceFrame, SLOT(indent()),
        QKeySequence("Ctrl+>"));
    editMenu->addAction(tr("Unindent &<"), sourceFrame, SLOT(unIndent()),
        QKeySequence("Ctrl+<"));
    editMenu->addSeparator();
    editMenu->addAction(tr("&Find"), sourceFrame, SLOT(find()),
        QKeySequence::Find);
    editMenu->addSeparator();
    editMenu->addAction(tr("Select &all"), sourceFrame, SLOT(selectAll()),
        QKeySequence::SelectAll);
    editMenu->addAction(tr("Select none &0"), sourceFrame, SLOT(selectNone()),
        QKeySequence("Ctrl+0"));
    editMenu->addSeparator();
    editMenu->addAction(tr("&Prettify"), sourceFrame, SLOT(prettify()));
    editMenu->addSeparator();
    editMenu->addAction(tr("&Edit settings"), this, SLOT(editSettings()));

    moveMenu = menuBar()->addMenu(tr("&Move"));
    moveMenu->addAction(tr("Page forward (&down)"), sourceFrame, SLOT(pageForward()),
        QKeySequence::MoveToNextPage);
    moveMenu->addAction(tr("Page backward (&up)"), sourceFrame, SLOT(pageBackward()),
        QKeySequence::MoveToPreviousPage);
    moveMenu->addAction(tr("Go to line &1"), sourceFrame, SLOT(gotoFirstLine()),
        QKeySequence::MoveToStartOfDocument);
    moveMenu->addAction(tr("Go to last lin&e"), sourceFrame,
        SLOT(gotoLastLine()), QKeySequence::MoveToEndOfDocument);
    moveMenu->addAction(tr("Go to &line n"), sourceFrame, SLOT(gotoLine()),
        QKeySequence("Ctrl+L"));
    moveMenu->addAction(tr("Go to &top of screen"), sourceFrame, SLOT(gotoTop()),
        QKeySequence("Ctrl+T"));
    moveMenu->addAction(tr("Go to &bottom"), sourceFrame, SLOT(gotoBottom()),
        QKeySequence("Ctrl+B"));
    moveMenu->addAction(tr("Move line to &middle"), sourceFrame, SLOT(center()),
        QKeySequence("Ctrl+M"));

    editToolBar->addAction(QIcon(QString(":/icons/%1/cut.png").arg(icon_size)),
        tr("Cut (Ctrl+X)"), sourceFrame, SLOT(cut()));
    editToolBar->addAction(QIcon(QString(":/icons/%1/copy.png").arg(icon_size)),
        tr("&Copy (Ctrl+C)"), sourceFrame, SLOT(copy()));
    editToolBar->addAction(
        QIcon(QString(":/icons/%1/paste.png").arg(icon_size)), tr("Paste (Ctrl+V)"),
        sourceFrame, SLOT(paste()));
    editToolBar->addSeparator();
    editToolBar->addAction(QIcon(QString(":/icons/%1/undo.png").arg(icon_size)),
        tr("Undo (Ctrl+Z)"), sourceFrame, SLOT(undo()));
    editToolBar->addAction(QIcon(QString(":/icons/%1/redo.png").arg(icon_size)),
        tr("Redo (Ctrl+Shift+Z)"), sourceFrame, SLOT(redo()));
    editToolBar->addSeparator();
    editToolBar->addAction(
        QIcon(QString(":/icons/%1/indent-more.png").arg(icon_size)),
        tr("Indent (Ctrl+>)"), sourceFrame, SLOT(indent()));
    editToolBar->addAction(
        QIcon(QString(":/icons/%1/indent-less.png").arg(icon_size)),
        tr("Unindent (Ctrl+<)"), sourceFrame, SLOT(unIndent()));
    editToolBar->addSeparator();
    editToolBar->addAction(QIcon(QString(":/icons/%1/find.png").arg(icon_size)),
        tr("Find (Ctrl+F)"), sourceFrame, SLOT(find()));
    editToolBar->addAction(
        QIcon(QString(":/icons/%1/select-all.png").arg(icon_size)),
        tr("Select all (Ctrl+A)"), sourceFrame, SLOT(selectAll()));
    editToolBar->addAction(
        QIcon(QString(":/icons/%1/prettify.png").arg(icon_size)),
        tr("Prettify (Ctrl+P)"), sourceFrame, SLOT(prettify()));
    editToolBar->addSeparator();
    editToolBar->addAction(
        QIcon(QString(":/icons/%1/font-decrease.png").arg(icon_size)),
        tr("Decrease font (Ctrl+-)"), this, SLOT(decreaseFont()));
    editToolBar->addAction(
        QIcon(QString(":/icons/%1/font-increase.png").arg(icon_size)),
        tr("Increase font (Ctrl++)"), this, SLOT(increaseFont()));

    addToolBar(Qt::TopToolBarArea, editToolBar);

    debugToolBar->addAction(QIcon(QString(":/icons/%1/ebe.png").arg(icon_size)),
        tr("Run (F5)"), sourceFrame, SLOT(run()));
    debugToolBar->actions()[0]->setShortcut(QKeySequence("F5"));
    debugToolBar->addAction(
        QIcon(QString(":/icons/%1/next.png").arg(icon_size)), tr("Next (F6)"),
        sourceFrame, SLOT(next()));
    debugToolBar->actions()[1]->setShortcut(QKeySequence("F6"));
    debugToolBar->addAction(
        QIcon(QString(":/icons/%1/step.png").arg(icon_size)), tr("Step (F7)"),
        sourceFrame, SLOT(step()));
    debugToolBar->actions()[2]->setShortcut(QKeySequence("F7"));
    debugToolBar->addAction(
        QIcon(QString(":/icons/%1/continue.png").arg(icon_size)),
        tr("Continue (F8)"), sourceFrame, SLOT(Continue()));
    debugToolBar->actions()[3]->setShortcut(QKeySequence("F8"));
    debugToolBar->addAction(
        QIcon(QString(":/icons/%1/process-stop.png").arg(icon_size)),
        tr("stop (F9)"), sourceFrame, SLOT(stop()));
    debugToolBar->actions()[4]->setShortcut(QKeySequence("F9"));

    addToolBar(Qt::TopToolBarArea, debugToolBar);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(dataDock->toggleViewAction());
    viewMenu->addAction(registerDock->toggleViewAction());
    if ( wordSize == 64 ) {
        viewMenu->addAction(halRegisterDock->toggleViewAction());
        viewMenu->addAction(halNamesDock->toggleViewAction());
    }
    viewMenu->addAction(asmDataDock->toggleViewAction());
    viewMenu->addAction(frameDock->toggleViewAction());
    viewMenu->addAction(floatDock->toggleViewAction());
    viewMenu->addAction(backTraceDock->toggleViewAction());
    viewMenu->addAction(consoleDock->toggleViewAction());
    viewMenu->addAction(terminalDock->toggleViewAction());
    viewMenu->addAction(projectDock->toggleViewAction());
    viewMenu->addAction(toyBoxDock->toggleViewAction());
    viewMenu->addAction(bitBucketDock->toggleViewAction());
    addToggle ( viewMenu, tr("T&ooltips"), this, SLOT(setTooltipsVisible(bool)),
        ebe["tooltips/visible"].toBool() );
    addToggle ( viewMenu, tr("Command &line"), sourceFrame,
        SLOT(setCommandLineVisible(bool)),
        ebe["command/visible"].toBool() );

    fontMenu = menuBar()->addMenu(tr("F&ont"));
    fontMenu->addAction(tr("&Increase font"), this, SLOT(increaseFont()),
        QKeySequence::ZoomIn);
    fontMenu->addAction(tr("&Decrease font"), this, SLOT(decreaseFont()),
        QKeySequence::ZoomOut);

    helpMenu = menuBar()->addMenu(tr("&Help "));
    helpAction(helpMenu, tr("&Getting started"), "start.html");
    helpAction(helpMenu, tr("&Projects"), "projects.html");
    helpAction(helpMenu, tr("&Editing"), "editing.html");
    helpAction(helpMenu, tr("&Breakpoints"), "breakpoints.html");
    helpAction(helpMenu, tr("&Running"), "running.html");
    helpAction(helpMenu, tr("&About"), "about.html");

    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/cin.png").arg(icon_size)), tr("cin: input"));
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/cout.png").arg(icon_size)),
        tr("cout: output"));
    templateToolBar->addSeparator();
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/if.png").arg(icon_size)), "if");
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/if-else.png").arg(icon_size)), "if-else");
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/switch.png").arg(icon_size)),
        tr("switch: multi-way branch"));
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/for.png").arg(icon_size)),
        tr("for: counting loop"));
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/while.png").arg(icon_size)),
        tr("while: general loop with test at the top"));
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/do-while.png").arg(icon_size)),
        tr("do-while: general loop with test at the bottom"));
    templateToolBar->addSeparator();
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/double.png").arg(icon_size)),
        tr("double: floating point number"));
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/int.png").arg(icon_size)),
        tr("int: an integer"));
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/string.png").arg(icon_size)), "string");
    templateToolBar->addSeparator();
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/main.png").arg(icon_size)), "main");
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/function.png").arg(icon_size)), "function");
    templateToolBar->addSeparator();
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/class.png").arg(icon_size)), "class");
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/struct.png").arg(icon_size)), "struct");
    templateToolBar->addAction(
        QIcon(QString(":/icons/%1/library.png").arg(icon_size)), tr("library"));

    connect ( templateToolBar, SIGNAL(actionTriggered(QAction*)),
        sourceFrame, SLOT(doTemplate(QAction*)) );
    addToolBar(Qt::RightToolBarArea, templateToolBar);
}

void MainWindow::open(QString name)
{
    if (name.endsWith(".ebe", Qt::CaseInsensitive)) {
        projectWindow->open(name);
    } else {
        sourceFrame->open(name, 0);
    }
}

void MainWindow::setTooltipsVisible(bool visible)
{
    ebe["tooltips/visible"] = visible;
    tooltipsVisible = visible;
}

QAction *MainWindow::addToggle(QMenu *menu, QString text, QObject *object,
    const char *slot, bool checked)
{
    QAction *action = new QAction(text, this);
    action->setCheckable(true);
    action->setChecked(checked);
    menu->addAction(action);
    connect ( action, SIGNAL(triggered(bool)), object, slot );
    return action;
}

void MainWindow::helpAction(QMenu *menu, QString text, QString file)
{
    QAction *action = new QAction(text, this);
    action->setData(file);
    menu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(displayHelp()));
}

void MainWindow::displayHelp()
{
    QAction *action = (QAction *)sender();
    QTextBrowser *text = new QTextBrowser;
    text->resize(900,800);
    text->setSource(QUrl("qrc:/html/" + action->data().toString()));
    text->setWindowTitle(action->data().toString());
    text->setFontPointSize( (double)fontSize );
    text->show();
}

void MainWindow::quit()
{
    if (sourceFrame->filesSaved()) {
#ifdef Q_OS_WIN32
        if ( needToKill ) TerminateProcess(hProcess,0);
#else
        if (gdbWaiting) {
            kill(gdbProcess->pid(), 2);
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

    //qDebug() << tr("Select working directory") << QDir::currentPath();
    dir = QFileDialog::getExistingDirectory(this,
        tr("Select working directory"), QDir::currentPath(),
        QFileDialog::ShowDirsOnly);
    if (dir != "") {
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


void MainWindow::initializePreferredWindowSize()
{
    QRect rect = QApplication::desktop()->availableGeometry();
    //qDebug() << "aval Geo" << rect;

    if (userSetGeometry && userHeight != 0) {
        rect.setHeight(userHeight);
    }
    if (userSetGeometry && userWidth != 0) {
        rect.setWidth(userWidth);
    }

    preferredWindowSize.setWidth(rect.width()*9/10);
    preferredWindowSize.setHeight(rect.height()*9/10);
}

void MainWindow::createDockWindows()
{
    asmDataWindow = new AsmDataWindow(this);

    dataDock = new QDockWidget(tr("&Data"));
    dataDock->setObjectName("Dock 1");
    dataDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    dataWindow = new DataWindow(dataDock);
    dataWindow->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    dataDock->setWidget(dataWindow);
    addDockWidget(Qt::LeftDockWidgetArea, dataDock);

    registerDock = new QDockWidget(tr("&Registers"));
    registerDock->setObjectName("Dock 2");
    registerDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    registerWindow = new RegisterWindow(this);
    registerWindow->setSizePolicy(QSizePolicy::Preferred,
        QSizePolicy::Preferred);
    registerDock->setWidget(registerWindow);
    addDockWidget(Qt::LeftDockWidgetArea, registerDock);

    if ( wordSize == 64 ) {
        halRegisterDock = new QDockWidget(tr("&HAL Registers"));
        halRegisterDock->setObjectName("Dock 10");
        halRegisterDock->setAllowedAreas(
            Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
                | Qt::BottomDockWidgetArea);
        halRegisterWindow = new HalRegisterWindow(this);
        halRegisterWindow->setSizePolicy(QSizePolicy::Preferred,
            QSizePolicy::Preferred);
        halRegisterDock->setWidget(halRegisterWindow);
        addDockWidget(Qt::LeftDockWidgetArea, halRegisterDock);

        halNamesDock = new QDockWidget(tr("HAL &Names"));
        halNamesDock->setObjectName("Dock 11");
        halNamesDock->setAllowedAreas(
            Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
                | Qt::BottomDockWidgetArea);
        halNamesWindow = new HalNamesWindow(this);
        halNamesWindow->setSizePolicy(QSizePolicy::Preferred,
            QSizePolicy::Preferred);
        halNamesDock->setWidget(halNamesWindow);
            addDockWidget(Qt::LeftDockWidgetArea, halNamesDock);
    }

    frameDock = new QDockWidget(tr("&Stack Frame"));
    frameDock->setObjectName("Dock 12");
    frameDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    frameWindow = new FrameWindow(this);
    frameWindow->setSizePolicy(QSizePolicy::Preferred,
        QSizePolicy::Preferred);
    frameDock->setWidget(frameWindow);
    addDockWidget(Qt::LeftDockWidgetArea, frameDock);

    asmDataDock = new QDockWidget(tr("&Assembly Data"));
    asmDataDock->setObjectName("Dock 13");
    asmDataDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    asmDataWindow->setSizePolicy(QSizePolicy::Preferred,
        QSizePolicy::Preferred);
    asmDataDock->setWidget(asmDataWindow);
    addDockWidget(Qt::LeftDockWidgetArea, asmDataDock);

    floatDock = new QDockWidget(tr("&Floating Point Registers"));
    floatDock->setObjectName("Dock 3");
    floatDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    floatWindow = new FloatWindow(floatDock);
    floatWindow->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    floatDock->setWidget(floatWindow);
    addDockWidget(Qt::LeftDockWidgetArea, floatDock);

    projectDock = new QDockWidget(tr("&Project"));
    projectDock->setObjectName("Dock 4");
    projectDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    projectWindow = new ProjectWindow(projectDock);
    projectWindow->setSizePolicy(QSizePolicy::Preferred,
        QSizePolicy::Preferred);
    projectDock->setWidget(projectWindow);
    addDockWidget(Qt::LeftDockWidgetArea, projectDock);

    backTraceDock = new QDockWidget(tr("&Back Trace"));
    backTraceDock->setObjectName("Dock 5");
    backTraceDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    backTraceWindow = new BackTraceWindow(backTraceDock);
    backTraceWindow->setSizePolicy(QSizePolicy::Preferred,
        QSizePolicy::Preferred);
    backTraceDock->setWidget(backTraceWindow);
    addDockWidget(Qt::LeftDockWidgetArea, backTraceDock);

    terminalDock = new QDockWidget(tr("&Terminal"));
    terminalDock->setObjectName("Dock 6");
    terminalDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    terminalWindow = new TerminalWindow(terminalDock);
    terminalDock->setWidget(terminalWindow);
    addDockWidget(Qt::LeftDockWidgetArea, terminalDock);

    toyBoxDock = new QDockWidget(tr("Toy Bo&x"));
    toyBoxDock->setObjectName("Dock 7");
    toyBoxDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    toyBox = new ToyBox(toyBoxDock);
    toyBoxDock->setWidget(toyBox);
    addDockWidget(Qt::LeftDockWidgetArea, toyBoxDock);

    bitBucketDock = new QDockWidget(tr("Bit B&ucket"));
    bitBucketDock->setObjectName("Dock 8");
    bitBucketDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    bitBucket = new BitBucket;
    bitBucketDock->setWidget(bitBucket);
    addDockWidget(Qt::LeftDockWidgetArea, bitBucketDock);

    consoleDock = new QDockWidget(tr("&Console"));
    consoleDock->setObjectName("Dock 9");
    consoleDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea
            | Qt::BottomDockWidgetArea);
    consoleWindow = new ConsoleWindow(consoleDock);
    consoleDock->setWidget(consoleWindow);
    addDockWidget(Qt::LeftDockWidgetArea, consoleDock);

    if ( wordSize == 64 ) {
        halNamesDock->setVisible(ebe["halnames/visible"].toBool());
        halRegisterDock->setVisible(ebe["halregister/visible"].toBool());
    }

    initializePreferredWindowSize();

    int availableHeight = preferredWindowSize.height();
    setVisibleIfFits(dataDock, "data/visible", &availableHeight);
    setVisibleIfFits(registerDock, "register/visible", &availableHeight);
    setVisibleIfFits(floatDock, "float/visible", &availableHeight);
    setVisibleIfFits(projectDock, "project/visible", &availableHeight);
    setVisibleIfFits(backTraceDock, "backtrace/visible", &availableHeight);
    setVisibleIfFits(terminalDock, "terminal/visible", &availableHeight);
    setVisibleIfFits(toyBoxDock, "toybox/visible", &availableHeight);
    setVisibleIfFits(bitBucketDock, "bitbucket/visible", &availableHeight);
    setVisibleIfFits(consoleDock, "console/visible", &availableHeight);
}

void MainWindow::setVisibleIfFits(QWidget *dockWidget, const QString &key, int *availableHeight)
{
    bool visible = ebe[key].toBool();
    int dockHeight = dockWidget->height();
    if (visible && dockHeight <= *availableHeight) {
        dockWidget->setVisible(true);
        *availableHeight -= dockHeight;
    } else {
        dockWidget->setVisible(false);
    }
}

void MainWindow::editSettings()
{
    ebe["ebe/geometry"] = saveGeometry();
    ebe["ebe/state"] = saveState();
    SettingsDialog *dialog = new SettingsDialog();
    if (dialog->exec()) restoreMainWindow();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //printf("key %x\n",event->key());
    if (event->matches(QKeySequence::ZoomIn)) {
        increaseFont();
    } else if (event->matches(QKeySequence::ZoomOut)) {
        decreaseFont();
    }
    event->ignore();
}
