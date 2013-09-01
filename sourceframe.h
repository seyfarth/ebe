#ifndef SOURCEFRAME_H
#define SOURCEFRAME_H

#include <QtGui>
#include "sourcewindow.h"
#include "types.h"

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
    void open(QString name, int index=-1);
    void openInNewTab(QString name);
    void closeTabs();
    void saveIfChanged(QString file);
    void setNextLine(QString &file,int &line);
    void clearNextLine(QString file,int line);
    QString buildDebugAsm(QString file);
    int  fontHeight;
    int  fontWidth;
    bool filesSaved();
    QLabel *cursorPosition;
    void close(QString);
    bool inAssembly;
    bool definesStart;

public slots:
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
    void indent();
    void unIndent();
    void find();
    void pageForward();
    void pageBackward();
    void center();
    void gotoFirstLine();
    void gotoLastLine();
    void gotoTop();
    void gotoBottom();
    void gotoLine();
    void prettify();
    void newFile();
    void templateC();
    void templateCpp();
    void templateAssembly();
    void templateFortran();
    void updateCursorPosition();
    void doTemplate(QAction*);
    void insertFile(QString f);

signals:
    void doRun(QString exe, QString options, QStringList files,
               QList<StringSet> breakpoints, QStringList globals);
    void doNext();
    void doNextInstruction();
    void doCall();
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
};

#endif
