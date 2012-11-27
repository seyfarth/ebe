#include "commandline.h"
#include "settings.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

/**
 *  \brief Constructor for the CommandLine class
 *
 *  The constructor uses a QHBoxLayout to place a QLabel and a QLineEdit
 *  in the QFrame which is the container for the widgets.  The label is
 *  placed to the left of the line edit widget. The line edit input field
 *  starts empty.
 */
CommandLine::CommandLine(QWidget *parent)
: QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Plain );
    setLineWidth(0);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(10);
    layout->setContentsMargins(5,5,5,5);
    QLabel *label = new QLabel(tr("Command line"));
    layout->addWidget(label);
    lineEdit = new QLineEdit();
    layout->addWidget(lineEdit);
    setLayout(layout);
    setVisible(ebe["command/visible"].toBool());
}

/**
 *  \brief This is a function to get the text in the line edit
 */
QString CommandLine::text()
{
    return lineEdit->text();
}
