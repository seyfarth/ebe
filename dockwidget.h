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
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

public slots:
    void changedTopLevel(bool floating);
    void changedVisibility(bool visible);
};

#endif
