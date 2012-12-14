#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <QPlainTextEdit>
#include <QFrame>
#include <QString>
#include <QLineEdit>
#include <QKeyEvent>

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

class TerminalWindow : public QFrame
{
    Q_OBJECT

public:
    TerminalWindow(QWidget *parent=0);
    QString ptyName;
    
private slots:
    void dataReady(QString data);
    void lineEditReady();

private:
    TerminalEdit *edit;
    QLineEdit *lineEdit;
    int pty;
    int ptySlave;
    PtyReader *ptyReader;
};

#endif
