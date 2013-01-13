#ifndef BACKTRACEWINDOW_H
#define BACKTRACEWINDOW_H

#include <QtGui>

class BackTraceWindow : public QFrame
{
    Q_OBJECT

public:
    BackTraceWindow(QWidget *parent=0);
    QStringList trace;
    int level;
    QSize sizeHint() const;
    QListWidget *list;
public slots:
    void receiveBackTrace(QStringList);

};

#endif
