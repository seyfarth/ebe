#include "backtracewindow.h"
#include "gdb.h"
#include <cstdio>

extern GDB *gdb;

BackTraceWindow::BackTraceWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Back trace");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(10,10,10,10);
    list = new QListWidget();
    layout->addWidget(list);
    setLayout(layout);
    connect ( gdb, SIGNAL(sendBackTrace(QStringList)),
              this, SLOT(receiveBackTrace(QStringList)) );
}

QSize BackTraceWindow::sizeHint() const
{
    return QSize(300,50);
}

void BackTraceWindow::receiveBackTrace(QStringList results)
{
    trace = results;
    level = results.length();
    list->clear();
    list->addItems(results);
}
