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
    setToolTip(tr("Left click on the titlebar to drag this subwindow.\n"
                  "If it stops moving then start dragging in the body\n"
                  "of the subwindow.  You can place it anywhere in the\n"
                  "left collection of subwindows or drop it on another\n"
                  "subwindow to merge into a tabbed window.\n\n"
                  "Dragging it outside the dock will make it a floating\n"
                  "window which can be resized and moved at will.\n\n"
                  "Right click to see the view options."));
    //connect ( this, SIGNAL(visibilityChanged(bool)),
              //this, SLOT(changedVisibility(bool)) );
    //connect ( this, SIGNAL(topLevelChanged(bool)),
              //this, SLOT(changedTopLevel(bool)) );
}

void DockWidget::changedVisibility(bool visible)
{
    QWidget *w;
    //qDebug() << "visibilityChanged" << visible << this;
    //qDebug() << mouseGrabber() << this;
    if ( visible && needToDock ) {
        needToDock = false;
        setFloating(false);
        w = widget();
        //qDebug() << "widgets" << w << dataWindow;
        //widget()->show();
        //widget()->update();
    }
    if ( isFloating() && !isVisible() ) {
       //qDebug() << "floating and not visible";
       needToDock = true;
       //setFloating(false);
       //setFloating(true);
    }
}

void DockWidget::changedTopLevel(bool /*floating*/)
{
    //qDebug() << "topLevelChanged" << floating;
    //qDebug() << mouseGrabber() << this;
    //if ( floating && needToDock ) {
        //needToDock = false;
    //}
}

void DockWidget::enterEvent(QEvent * /*event*/)
{
    //qDebug() << "entered" << this;
}

void DockWidget::leaveEvent(QEvent * /*event*/)
{
    //qDebug() << "left, floating" << isFloating() << this;
    //if ( isFloating() && !isVisible() ) {
       //qDebug() << "floating and not visible";
       //needToDock = true;
       //setFloating(false);
       //setFloating(true);
    //} else if ( isFloating() ) {
       //qDebug() << "floating and visible";
       needToDock = true;
    //}
}
