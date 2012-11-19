#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QFrame>
#include <QString>
#include "sourcewindow.h"

QT_BEGIN_NAMESPACE
class QWidget;
class QLineEdit;
QT_END_NAMESPACE

class CommandLine : public QFrame
{
    Q_OBJECT

public:
    CommandLine(QWidget *parent=0);
    QString text();

private slots:


private:
    QLineEdit *lineEdit;

};

#endif
