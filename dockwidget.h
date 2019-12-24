#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include "types.h"
#include <QEvent>
#include <QDockWidget>

class DockWidget: public QDockWidget
{
    Q_OBJECT

public:
    bool needToDock;
    DockWidget(QString s);
    bool event(QEvent *event);
};

#endif
