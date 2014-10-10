#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QFrame>
#include <QString>

class QLineEdit;

/**
 *  \class CommandLine
 *
 *  \brief Class to handle command line parameters for a program being debugged
 *
 *  The \c CommandLine class is derived from the \c QFrame class.  It consists
 *  of a \c QLineEdit which allows the user to enter command line parameters
 *  and \c QLabel which describes the purpose of the \c QLineEdit.
 *
 *  It has a public function, text, which is used to get the current text
 *  of the \c QLineEdit.
 */
class CommandLine: public QFrame
{
    Q_OBJECT

public:
    CommandLine(QWidget *parent = 0);
    QString text();

private:
    QLineEdit *lineEdit;    ///< Widget allowing editing

};

#endif
