#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "sourcewindow.h"


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);

private slots:
    void increaseFont();
    void decreaseFont();
    void displayHelp();
    void quit();


private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void createDockWindows();
    void helpAction ( QMenu *menu, QString s, QString file );
    void keyPressEvent ( QKeyEvent *event );

    SourceWindow *source;

    int fontSize;

    QMenu *fileMenu;
    QMenu *templateMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *moveMenu;
    QMenu *fontMenu;
    QMenu *helpMenu;

//  Actions for the file menu

    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *closeAction;
    QAction *saveAsAction;
    QAction *newProjectAction;
    QAction *openProjectAction;
    QAction *closeProjectAction;
    QAction *saveSettingsAction;
    QAction *saveSettingsAsAction;
    QAction *quitAction;
    QAction *templateCAction;
    QAction *templateCppAction;
    QAction *templateFortranAction;
    QAction *templateAssemblyAction;

//  Actions for the edit menu

    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;;
    QAction *undoAction;
    QAction *redoAction;
    QAction *commentAction;
    QAction *uncommentAction;
    QAction *indentAction;
    QAction *unindentAction;
    QAction *findAction;
    QAction *selectAllAction;
    QAction *selectNoneAction;
    QAction *prettifyAction;

//  Actions for the move menu

    QAction *pageFwdAction;
    QAction *pageBackAction;
    QAction *firstLineAction;
    QAction *lastLineAction;
    QAction *gotoLineAction;
    QAction *gotoTopAction;
    QAction *gotoBottomAction;
    QAction *centerAction;

//  Actions for the view menu
    QAction *dataAction;
    QAction *registerAction;
    QAction *floatAction;
    QAction *consoleAction;
    QAction *projectAction;
    QAction *tooltipsAction;


//  Actions for the font menu
    QAction *increaseAction;
    QAction *decreaseAction;

};

#endif
