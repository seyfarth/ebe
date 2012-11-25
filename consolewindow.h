#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QFrame>
#include <QString>

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QLineEdit;
QT_END_NAMESPACE

class ConsoleWindow : public QFrame
{
    Q_OBJECT

public:
    ConsoleWindow(QWidget *parent=0);

private slots:

private:
    QPlainTextEdit *textEdit;
    QLineEdit *commandLine;
};

#endif
