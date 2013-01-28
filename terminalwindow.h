#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <QPlainTextEdit>
#include <QFrame>
#include <QString>
#include <QTimer>
#include <QPalette>
#include <QColor>
#include <QLineEdit>
#include <QKeyEvent>
#ifdef Q_WS_WIN
#include <windows.h>
#endif

class PtyReader;

class TerminalEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    TerminalEdit(int fd, QWidget *parent=0);

private slots:

private:
    int pty;

};

class InputEdit : public QLineEdit
{
    Q_OBJECT 

public:
    InputEdit();

private:
    void keyPressEvent(QKeyEvent *e);
    void startFlash();
    QTimer *timer;
    int flashCount;

public slots:
    void endFlash();
    void updateFlash();

signals:
    void sendEOF();

};

class TerminalWindow : public QFrame
{
    Q_OBJECT

public:
    TerminalWindow(QWidget *parent=0);
    QString ptyName;
    
private slots:
    void dataReady(QString data);
    void lineEditReady();

public:
    TerminalEdit *edit;
    InputEdit *lineEdit;
#ifdef Q_WS_WIN
    HANDLE toChild;
    HANDLE fromChild;
    HANDLE childStdin;
    HANDLE childStdout;
#else

#endif
    int pty;
    int ptySlave;
    PtyReader *ptyReader;

};

#endif
