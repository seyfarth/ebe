#ifndef SOURCEEDIT_H
#define SOURCEEDIT_H

#include <QPlainTextEdit>
#include <QString>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QLineEdit;
class QPushButton;
class QKeyEvent;
class QEvent;
class QWheelEvent;
class QScrollBar;
class QResizeEvent;
QT_END_NAMESPACE

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

#endif
