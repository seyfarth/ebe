#ifndef SOURCEFRAME_H
#define SOURCEFRAME_H

#include <QtGui>
#include "sourcewindow.h"

class CommandLine;

class SourceFrame : public QFrame
{
    Q_OBJECT

public:
    SourceFrame(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    void setLineNumbers(int nLines);
    bool fileChanged();
    void saveBeforeQuit();
    void open(QString name, int index=0);
    void openInNewTab(QString name);
    void closeTabs();
    QStringList fortranExts;
    QStringList cExts;
    QStringList cppExts;
    QStringList asmExts;
    void saveIfChanged(QString file);

private slots:
    void setCommandLineVisible(bool);
    void changedTab(int index);
    void open();
    void save();
    void saveAs();
    void close();
    void run();

signals:
    void doRun(QString exe, QStringList files, QList<IntSet> breakpoints);

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
