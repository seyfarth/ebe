#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <QPlainTextEdit>
#include <QString>

QT_BEGIN_NAMESPACE
class QKeyEvent;
QT_END_NAMESPACE

class TerminalWindow : public QPlainTextEdit
{
    Q_OBJECT

public:
    TerminalWindow(QWidget *parent=0);

private slots:

private:
    void keyPressEvent ( QKeyEvent *event );

};

#endif
