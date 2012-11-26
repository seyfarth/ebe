#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include <QFrame>
#include <QString>

#include <QTextDocument>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QLineEdit;
class QPushButton;
class QKeyEvent;
QT_END_NAMESPACE

class SourceEdit;
class CommandLine;

class SourceWindow : public QFrame
{
    Q_OBJECT

public:
    SourceWindow(QWidget *parent=0);
    void setLineNumberWidth(int width);
    void setLineNumbers(int nLines);
    bool fileChanged();
    void saveBeforeQuit();

private slots:
    void setCommandLineVisible(bool);
    void open();
    void save();
    void prepareLineNumberEdit();

private:
    void createLineNumberEdit();
    void createTextEdit();
    void createButtons();
    void createCommandLineEdit();

    SourceEdit *textEdit;
    QTextDocument *textDoc;
    QPlainTextEdit *lineNumberEdit;
    bool changed = false;
    QString openedFileName;

    QPushButton *quitButton;
    QPushButton *runButton;
    QPushButton *nextButton;
    QPushButton *stepButton;
    QPushButton *continueButton;
    QPushButton *stopButton;

    CommandLine *commandLine;
};

#endif
