#ifndef BACKTRACEWINDOW_H
#define BACKTRACEWINDOW_H

#include <QtGui>
#include <QListWidget>

/**
 *  \class BackTraceWindow
 *
 *  \brief Class to display the results of the \c gdb \c backtrace command
 *
 *  This class is derived from the \c QFrame class and, like the other
 *  dock widgets, it has a beveled border. The only widget displayed in
 *  the frame is a \c QListWidget which shows the data from the \c gdb
 *  \c backtrace command.
 *
 *  The \c gdb \c backtrace command examines the the stack and prints the
 *  history of function calls leading to the current point in execution.
 *  The first line of output shows the next line to execute in the current
 *  function and each line below shows where a function call was made.
 */
class BackTraceWindow : public QFrame
{
    Q_OBJECT

public:
    BackTraceWindow(QWidget *parent=0);
    QStringList trace;      ///< A copy of the most recent \c backtrace command
    int level;              ///< The number of calls made in the \c backtrace
    QSize sizeHint() const;
    QListWidget *list;      ///< The widget displaying the \c backtrace
public slots:
    void receiveBackTrace(QStringList);

};

#endif
