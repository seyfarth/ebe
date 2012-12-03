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
    //void scrollContentsBy(int dx, int dy);
    bool event ( QEvent *e );
    void wheelEvent ( QWheelEvent *e );
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

public slots:
    void open();
    void save();
    void textChanged();
    void newHeight(int heightInPixels);
    void scrollBarChanged(int value);

private:
    void createLineNumberEdit();
    void createTextEdit();
    void createButtons();
    void createCommandLineEdit();

    int numLines;
    int textHeight;
    int heightInPixels;
    int fontWidth;
    int fontHeight;
    int topNumber;
    int bottomNumber;
    SourceEdit *textEdit;
    QTextDocument *textDoc;
    LineNumberEdit *lineNumberEdit;
    bool changed;
    QString openedFileName;
    QScrollBar *scrollBar;

};

#endif
