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
    void saveIfChanged(QString file);
    void setNextLine(QString file,int line);
    void clearNextLine(QString file,int line);

private slots:
    void nextInstruction(QString file,int line);
    void setCommandLineVisible(bool);
    void changedTab(int index);
    void open(bool);
    void save();
    void saveAs();
    void close();
    void run();
    void next();
    void step();
    void Continue();
    void stop();
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    void selectAll();
    void selectNone();
    void comment();
    void unComment();

signals:
    void doRun(QString exe, QString options, QStringList files,
               QList<IntSet> breakpoints, QStringList globals);
    void doNext();
    void doStep();
    void doContinue();
    void doStop();

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

    QString breakFile;
    int breakLine;
};

#endif
