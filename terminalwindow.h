#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <QPlainTextEdit>
#include <QFrame>
#include <QString>
#include <QKeyEvent>

class TerminalEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    TerminalEdit(QWidget *parent=0);

private slots:

private:
    void keyPressEvent ( QKeyEvent *event );

};

class TerminalWindow : public QFrame
{
    Q_OBJECT

public:
    TerminalWindow(QWidget *parent=0);
    
private slots:

private:
    TerminalEdit *edit;
};

#endif
