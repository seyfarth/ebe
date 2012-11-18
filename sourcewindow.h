#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTextEdit;
class QLineEdit;
class QPushButton;
class QFrame;
QT_END_NAMESPACE

//! [0]
class SourceWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:

private:
    void createLineNumberEdit();
    void createTextEdit();
    void createButtons();
    void createCommandLineEdit();

    QFrame *frame;
    
    QTextEdit *textEdit;
    QTextEdit *lineNumberEdit;

    QPushButton *quitButton;
    QPushButton *runButton;
    QPushButton *nextButton;
    QPushButton *stepButton;
    QPushButton *continueButton;
    QPushButton *stopButton;

    QLineEdit *commandLineEdit;
};
//! [0]

#endif
