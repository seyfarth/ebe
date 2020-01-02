#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "types.h"
#include <QMainWindow>
#include <QEvent>
#include <QStatusBar>
#include "datawindow.h"
#include "sourceframe.h"
#include "registerwindow.h"
#include "framewindow.h"
#include "asmdatawindow.h"
#include "floatwindow.h"
#include "terminalwindow.h"
#include "consolewindow.h"
#include "projectwindow.h"
#include "backtracewindow.h"
#include "debugger.h"
#include "gdb.h"
#include "toybox.h"
#include "bitbucket.h"
#include "settings.h"
#include "dockwidget.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void saveSettings();
    bool toolExists(QString t);
    void open(QString name);
    static void setWordSize();

public slots:
    void restoreMainWindow();
    void increaseFont();
    void decreaseFont();
    void displayHelp();
    void setTooltipsVisible(bool);
    void quit();
    void editSettings();
    void changeDirectory();
    void doKill();

    signals:
    void sendWorkingDir(QString);
    void doStop();
    void doNextInstruction();

private:
    void closeEvent(QCloseEvent *e);
    void createMenus();
    void createStatusBar();
    void createDockWindows();
    void helpAction(QMenu *menu, QString s, QString file);
    void keyPressEvent(QKeyEvent *event);
    QAction *addToggle(QMenu *menu, QString text, QObject *object,
        const char *slot, bool checked);
    bool eventFilter(QObject *object, QEvent *event);
    void checkTools();
    void initializePreferredWindowSize();
    void setVisibleIfFits(QWidget *dockWidget, const QString &key, int *availableHeight);
    
    bool tooltipsVisible;

    DockWidget *dataDock;
    DockWidget *registerDock;
    DockWidget *frameDock;
    DockWidget *asmDataDock;
    DockWidget *halRegisterDock;
    DockWidget *halNamesDock;
    DockWidget *floatDock;
    DockWidget *projectDock;
    DockWidget *terminalDock;
    DockWidget *consoleDock;
    DockWidget *backTraceDock;
    DockWidget *toyBoxDock;
    DockWidget *bitBucketDock;

    QAction *dataVisible;
    QAction *registerVisible;
    QAction *halRegisterVisible;
    QAction *halNameVisible;
    QAction *frameVisible;
    QAction *asmDataVisible;
    QAction *floatVisible;
    QAction *projectVisible;
    QAction *terminalVisible;
    QAction *consoleVisible;

    void setFontSize();
    int fontSize;
    int cursorFlashTime;

    QMenu *fileMenu;
    QMenu *templateMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *moveMenu;
    QMenu *fontMenu;
    QMenu *helpMenu;

    QSize preferredWindowSize;
};

#endif
