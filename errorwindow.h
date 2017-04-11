#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QFrame>
#include <QPointer>
#include <QSize>

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QPushButton;
QT_END_NAMESPACE

class ErrorWindow: public QFrame
{
    Q_OBJECT

public:
    ErrorWindow(QWidget *parent = 0);
    QSize sizeHint() const;
    QPlainTextEdit *textEdit;

private slots:

private:
    QPushButton *button;
};

#ifdef ERRORWINDOW_CPP
QPointer<ErrorWindow> errorWindow;
#else
extern QPointer<ErrorWindow> errorWindow;
#endif

#endif
