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
    setToolTip(tr(
       "Drag the titlebar to move a subwindow anywhere on the left, right,\n"
       "top or bottom or drop on another one to merge into a tabbed window.\n"
       "Right click to see the view options."));
    //setToolTipDuration(4000);
}

#if 0
bool DockWidget::event(QEvent *event)
{
    if ( event->type() == QEvent::ToolTip ) {
        //qDebug() << "event" << this;
        //qDebug() << "cursor" << mapFromGlobal(cursor().pos());
        if ( mapFromGlobal(cursor().pos()).y() > 20 ) {
            //event->ignore();
            //return true;
        }
    }
    return QWidget::event(event);
}
#endif
