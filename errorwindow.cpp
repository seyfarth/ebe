#define ERRORWINDOW_CPP
#include "errorwindow.h"
#include "settings.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

ErrorWindow::ErrorWindow(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("Errors");
    setWindowTitle(tr("Errors"));
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    button = new QPushButton("OK");
    buttonLayout->addStretch();
    buttonLayout->addWidget(button);
    buttonLayout->addStretch();

    textEdit = new QPlainTextEdit(this);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);
    layout->addLayout(buttonLayout);
    setLayout(layout);
    show();
    connect(button, SIGNAL(clicked()), this, SLOT(close()));
}

QSize ErrorWindow::sizeHint() const
{
    return QSize(600, 400);
}
