#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "types.h"
#include <QMainWindow>
#include <QEvent>
#include <QStatusBar>
#include "datawindow.h"
#include "sourceframe.h"
#include "registerwindow.h"
#include "floatwindow.h"
#include "terminalwindow.h"
#include "consolewindow.h"
#include "projectwindow.h"
#include "backtracewindow.h"
#include "gdb.h"
#include "toybox.h"
#include "bitbucket.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QDockWidget;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);
    void saveSettings();
    void open(QString name);

public slots:
    void restoreMainWindow();
    void increaseFont();
    void decreaseFont();
    void displayHelp();
    void setTooltipsVisible(bool);
    void quit();
    void editSettings();
    void changeDirectory();

signals:
    void sendWorkingDir(QString);
    void doStop();

private:
    void closeEvent ( QCloseEvent *e );
    void createMenus();
    void createStatusBar();
    void createDockWindows();
    void helpAction ( QMenu *menu, QString s, QString file );
    void keyPressEvent ( QKeyEvent *event );
    QAction *addToggle ( QMenu *menu, QString text, QObject *object,
                     const char *slot, bool checked );
    bool eventFilter ( QObject *object, QEvent *event );
    void checkTools();
    bool toolExists(QString t);

    bool tooltipsVisible;

    QDockWidget *dataDock;
    QDockWidget *registerDock;
    QDockWidget *halRegisterDock;
    QDockWidget *floatDock;
    QDockWidget *projectDock;
    QDockWidget *terminalDock;
    QDockWidget *consoleDock;
    QDockWidget *backTraceDock;
    QDockWidget *toyBoxDock;
    QDockWidget *bitBucketDock;

    QAction *dataVisible;
    QAction *registerVisible;
    QAction *floatVisible;
    QAction *projectVisible;
    QAction *terminalVisible;
    QAction *consoleVisible;

    void setFontSize();
    int fontSize;

    QMenu *fileMenu;
    QMenu *templateMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *moveMenu;
    QMenu *fontMenu;
    QMenu *helpMenu;
};

#endif
