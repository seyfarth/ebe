#ifndef SOURCEWINDOW_H
#define SOURCEWINDOW_H

#include <QFrame>
#include <QString>

#include <QTextDocument>
#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QLineEdit;
class QPushButton;
class QKeyEvent;
class QScrollBar;
QT_END_NAMESPACE

class SourceEdit;
class CommandLine;

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

private slots:
    void setCommandLineVisible(bool);
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

    QPushButton *quitButton;
    QPushButton *runButton;
    QPushButton *nextButton;
    QPushButton *stepButton;
    QPushButton *continueButton;
    QPushButton *stopButton;

    CommandLine *commandLine;
};

#endif
