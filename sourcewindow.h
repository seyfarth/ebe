#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include <QtGui>
#include "variable.h"

class SourceEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    SourceEdit(QWidget *parent=0);
    int heightInPixels;

private slots:
    void printScroll();
    void defineVariable();
    //void receiveVariableDefinition(bool,QStringList);

signals:
    void newHeight(int height);
    void sendVariableDefinition(QStringList);

private:
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent ( QResizeEvent *e );
    void scrollContentsBy(int dx, int dy);
    //bool event ( QEvent *e );
    //void wheelEvent ( QWheelEvent *e );
    void contextMenuEvent ( QContextMenuEvent *event );
    int top;
    QScrollBar *scrollBar;
};



class LineNumberEdit: public QPlainTextEdit
{
    Q_OBJECT

public:
    LineNumberEdit(QWidget *parent=0);

    void wheelEvent ( QWheelEvent *e );
    QScrollBar *scrollBar;
    QTextBlockFormat normalFormat;
    QTextBlockFormat breakFormat;

private:
    QSet<int> *breakpoints;
    void mouseReleaseEvent ( QMouseEvent *event );
    void contextMenuEvent ( QContextMenuEvent *event );
    QPoint eventPosition;

private slots:
    void setBreakpoint();
    void dropBreakpoint();
    void dropAllBreakpoints();
    void ignore();
};


typedef QSet<int> IntSet;

class SourceWindow : public QFrame
{
    Q_OBJECT

public:
    SourceWindow(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    void setLineNumbers(int nLines);
    bool fileChanged();
    void saveBeforeQuit();
    QString fileName;
    bool changed;
    bool saved;
    bool opened;
    void open(QString name);
    SourceEdit *textEdit;
    QTextDocument *textDoc;
    int numLines;
    int textHeight;
    int heightInPixels;
    int fontWidth;
    int fontHeight;
    int topNumber;
    int bottomNumber;
    int lastLineNumber;
    IntSet *breakpoints;
    void setNextLine(int line);
    void clearNextLine(int line);
    QTextBlockFormat normalFormat;
    QTextBlockFormat breakFormat;
    void comment();
    void unComment();

public slots:
    void open();
    void save();
    void saveAs();
    void textChanged();
    void newHeight(int heightInPixels);
    void scrollBarChanged(int value);
    void scrollBarRangeChanged(int min, int max);
    void sliderChanged(int value);

private:
    void createLineNumberEdit();
    void createTextEdit();
    void createButtons();
    void createCommandLineEdit();

    LineNumberEdit *lineNumberEdit;
    QScrollBar *scrollBar;

signals:
};

#endif
