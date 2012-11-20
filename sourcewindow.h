#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include <QFrame>
#include <QString>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QLineEdit;
class QPushButton;
class QKeyEvent;
QT_END_NAMESPACE

class CommandLine;

class SourceWindow : public QFrame
{
    Q_OBJECT

public:
    SourceWindow(QWidget *parent=0);

private slots:
    void setCommandLineVisible(bool);

private:
    void createLineNumberEdit();
    void createTextEdit();
    void createButtons();
    void createCommandLineEdit();

    QTextEdit *textEdit;
    QTextEdit *lineNumberEdit;

    QPushButton *quitButton;
    QPushButton *runButton;
    QPushButton *nextButton;
    QPushButton *stepButton;
    QPushButton *continueButton;
    QPushButton *stopButton;

    CommandLine *commandLine;
};

#endif
