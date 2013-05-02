#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include <QtGui>
#include "variable.h"
#include "highlighter.h"

class SourceEdit;
class SourceWindow;

class FindReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    FindReplaceDialog();
    QLineEdit *findEdit;
    QLineEdit *replaceEdit;
    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *cancelButton;
    SourceEdit *textEdit;

public slots:
    void find();
    void replace();

private:
};

class LineNumberDialog: public QDialog
{
    Q_OBJECT

public:
    LineNumberDialog();
    QSpinBox *lineSpin;
    void setMax(int max);
    int line;
    QPushButton *okButton;
    QPushButton *cancelButton;

public slots:
    void setLine();
};
    
class SourceEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    SourceEdit(QWidget *parent=0);
    int heightInPixels;
    int tab_width;
    void setCompleter(QCompleter *c);
    QCompleter *completer() const;
    QStringListModel model;
    void addWords(QString t);
    QSet<QString> wordsInList;
    int complete_minimum;
    Highlighter *highlighter;
    bool autoIndent;
    void newLine();
    void indentNewLine(int k=0);

private slots:
    void printScroll();
    void defineVariable();
    void insertCompletion(const QString &completion);
    //void receiveVariableDefinition(bool,QStringList);

signals:
    void newHeight(int height);
    void sendVariableDefinition(QStringList);

private:
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e); 
    void resizeEvent ( QResizeEvent *e );
    void scrollContentsBy(int dx, int dy);
    //bool event ( QEvent *e );
    //void wheelEvent ( QWheelEvent *e );
    void contextMenuEvent ( QContextMenuEvent *event );
    QString textUnderCursor() const;

private:
    int top;
    QScrollBar *scrollBar;
    QCompleter *c;
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
    SourceWindow *myParent;

private slots:
    void setBreakpoint();
    void dropBreakpoint();
    void dropAllBreakpoints();
    void ignore();

signals:
    void sendBreakpoint(QString,QString);
    void deleteBreakpoint(QString,QString);
};


typedef QSet<int> IntSet;

class SourceWindow : public QFrame
{
    Q_OBJECT

public:
    SourceWindow(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    void setLineNumbers(int nLines);
    void saveCursor();
    void restoreCursor();
    bool fileChanged();
    void saveBeforeQuit();
    QString fileName;
    QString language;
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
    int tab_width;
    IntSet *breakpoints;
    void setNextLine(int line);
    void clearNextLine(int line);
    QTextBlockFormat normalFormat;
    QTextBlockFormat breakFormat;
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
    void doTemplate(QAction *a);
    void insertFile(QString f);
    QString languageFromFile(QString file);

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
