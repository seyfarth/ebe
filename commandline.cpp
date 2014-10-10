#include "commandline.h"
#include "settings.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

/**
 *  \brief Constructor for the \c CommandLine class
 *
 *  The constructor uses a \c QHBoxLayout to place a QLabel and a \c QLineEdit
 *  in the \c QFrame which is the container for the widgets.  The label is
 *  placed to the left of the line edit widget. The line edit input field
 *  starts empty.
 */
CommandLine::CommandLine(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Plain);
    setLineWidth(0);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(10);
    layout->setContentsMargins(5, 5, 5, 5);
    QLabel *label = new QLabel(tr("Command line"));
    layout->addWidget(label);
    lineEdit = new QLineEdit();
    lineEdit->setToolTip(tr("Enter extra parameters for the command line\n"
        "for your program when it runs.  The command\n"
        "line parameters are the parameters to main\n"
        "in C and C++(argc and argv).  The first is\n"
        "always the name of the program."));
    layout->addWidget(lineEdit);
    setLayout(layout);
    setVisible(ebe["command/visible"].toBool());
}

/**
 *  \brief This is a function to get the text in the line edit
 *
 *  \return QString holding the text from the \c QLineEdit
 */
QString CommandLine::text()
{
    return lineEdit->text();
}
