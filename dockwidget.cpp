#include <QtGui>
#include <QtGlobal>
#include <QDebug>

#include "mainwindow.h"
#include "datawindow.h"
#include "dockwidget.h"

extern MainWindow *mainWin;
extern DataWindow *dataWindow;
extern SourceFrame *sourceFrame;
extern RegisterWindow *registerWindow;
extern FrameWindow *frameWindow;
extern AsmDataWindow *asmDataWindow;
extern HalRegisterWindow *halRegisterWindow;
extern HalNamesWindow *halNamesWindow;
extern FloatWindow *floatWindow;
extern ProjectWindow *projectWindow;
extern TerminalWindow *terminalWindow;
extern ConsoleWindow *consoleWindow;
extern BackTraceWindow *backTraceWindow;
extern BitBucket *bitBucket;

DockWidget::DockWidget(QString name)
    : QDockWidget(name)
{
    needToDock = false;
    setToolTip(tr("Left click on the titlebar to drag a subwindow.\n"
                  "Click on the float icon (2 boxes) to make it visible\n"
                  "outside the ebe window. Then move or resize as desired.\n"
                  "Move a subwindow by dragging on the title bar.  You can.\n"
                  "place it anywhere in the left, right, top or bottom or\n"
                  "drop it on another one to merge into a tabbed window.\n"
                  "If dragging is difficult, float the subwindow first.\n"
                  "Right click to see the view options."));
    //setToolTipDuration(4000);
}

bool DockWidget::event(QEvent *event)
{
    if ( event->type() == QEvent::ToolTip ) {
        //qDebug() << "event" << this;
        //qDebug() << "cursor" << mapFromGlobal(cursor().pos());
        if ( mapFromGlobal(cursor().pos()).y() > 20 ) {
            event->accept();
            return true;
        }
    }
    QWidget::event(event);
    return false;
}
