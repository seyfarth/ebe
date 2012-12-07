#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include <QtGui>

class SourceEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    SourceEdit(QWidget *parent=0);
    int heightInPixels;

private slots:
    void printScroll();

signals:
    void newHeight(int height);

private:
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent ( QResizeEvent *e );
    void scrollContentsBy(int dx, int dy);
    //bool event ( QEvent *e );
    //void wheelEvent ( QWheelEvent *e );
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

private:
    QSet<int> *breakpoints;
    void mouseReleaseEvent ( QMouseEvent *event );
};


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
    QSet<int> breakpoints;

public slots:
    void open();
    void save();
    void saveAs();
    void textChanged();
    void newHeight(int heightInPixels);
    //void scrollBarChanged(int value);

private:
    void createLineNumberEdit();
    void createTextEdit();
    void createButtons();
    void createCommandLineEdit();

    LineNumberEdit *lineNumberEdit;
    QScrollBar *scrollBar;

};

#endif
