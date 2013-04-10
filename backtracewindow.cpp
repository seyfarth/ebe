#include "backtracewindow.h"
#include "gdb.h"
#include <cstdio>

extern GDB *gdb;

/**
 *  \brief Constructor for the \c BackTraceWindow class.
 *
 *  The constructor calls the \c QFrame constructor to get things
 *  in order before customizing.  It establishes a beveled border
 *  and then places its \c QListWidget in its layout.
 *
 *  Its final action is to connect the \c sendBackTrace signal
 *  from the \c gdb thread to its \c receiveBackTrace slot.
 */
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

/**
 *  \fn BackTraceWindow::sizeHint
 *
 *  This function gives a hint to the Qt layout engine on the minimum
 *  size required.  The 50 rows is not likely to be enough, but at least
 *  the widget can be shrunk a lot.
 */
QSize BackTraceWindow::sizeHint() const
{
    return QSize(200,10);
}

/**
 *  \fn BackTraceWindow::receiveBackTrace
 *
 *  This function is called with the \c gdb thread emits a signal providing
 *  new data to be displayed in the \c BackTraceWindow.
 *
 *  \param results The results from the \c gdb \c backtrace command.
 */
void BackTraceWindow::receiveBackTrace(QStringList results)
{
    trace = results;
    level = results.length();
    list->clear();
    list->addItems(results);
}
