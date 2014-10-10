#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QFrame>
#include <QString>

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QLineEdit;
class QScrollBar;
QT_END_NAMESPACE

class ConsoleWindow: public QFrame
{
    Q_OBJECT

public:
    ConsoleWindow(QWidget *parent = 0);

private slots:
    void log(QString);
    void sendCommand();

    signals:
    void doCommand(QString);

private:
    QPlainTextEdit *textEdit;
    QLineEdit *commandLine;
    QScrollBar *scrollBar;
};

#endif
