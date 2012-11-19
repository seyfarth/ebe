#include "commandline.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

CommandLine::CommandLine(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Plain );
    setLineWidth(0);

    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *label = new QLabel(tr("Command line"));
    layout->addWidget(label);
    lineEdit = new QLineEdit();
    layout->addWidget(lineEdit);
    setLayout(layout);
}

QString CommandLine::text()
{
    return lineEdit->text();
}
