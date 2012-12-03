#ifndef SOURCEFRAME_H
#define SOURCEFRAME_H

#include <QtGui>

class CommandLine;
class SourceWindow;

class SourceFrame : public QFrame
{
    Q_OBJECT

public:
    SourceFrame(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    void setLineNumbers(int nLines);
    bool fileChanged();
    void saveBeforeQuit();

private slots:
    void setCommandLineVisible(bool);
    void open();
    void save();

private:
    QTabWidget *tab;

    SourceWindow *source;

    QPushButton *quitButton;
    QPushButton *runButton;
    QPushButton *nextButton;
    QPushButton *stepButton;
    QPushButton *continueButton;
    QPushButton *stopButton;

    CommandLine *commandLine;
};

#endif
