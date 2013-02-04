#include "variable.h"

DefineVariableDialog::DefineVariableDialog()
: QDialog()
{
    setObjectName("Define Variable");
    setWindowTitle("Define Variable");
    //setFrameStyle ( QFrame::Panel | QFrame::Raised );
    //setLineWidth(4);
    setModal(true);
    setWindowFlags(Qt::Dialog|Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10,10,10,10);

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget ( new QLabel(tr("Name")) );
    nameEdit = new QLineEdit;
    nameLayout->addWidget ( nameEdit );
    layout->addLayout(nameLayout);

    QHBoxLayout *addressLayout = new QHBoxLayout;
    addressLayout->addWidget ( new QLabel(tr("Address")) );
    addressEdit = new QLineEdit;
    addressLayout->addWidget ( addressEdit );
    layout->addLayout(addressLayout);

    QStringList formats;
    formats << "Hexadecimal" << "Decimal" << "Unsigned decimal" << "Floating point"
            << "Character" << "String" << "String array";
    QHBoxLayout *formatLayout = new QHBoxLayout;
    formatLayout->addWidget ( new QLabel(tr("Format")) );
    formatCombo = new QComboBox;
    formatCombo->addItems(formats);
    formatLayout->addWidget ( formatCombo );
    layout->addLayout(formatLayout);

    QStringList sizes;
    sizes << "8" << "4" << "2" << "1";
    QHBoxLayout *sizeLayout = new QHBoxLayout;
    sizeLayout->addWidget ( new QLabel(tr("Size")) );
    sizeCombo = new QComboBox;
    sizeCombo->addItems(sizes);
    sizeLayout->addWidget ( sizeCombo );
    layout->addLayout(sizeLayout);

    QHBoxLayout *firstLayout = new QHBoxLayout;
    firstLayout->addWidget ( new QLabel(tr("First")) );
    firstEdit = new QLineEdit;
    firstEdit->setText("0");
    firstLayout->addWidget ( firstEdit );
    layout->addLayout(firstLayout);

    QHBoxLayout *lastLayout = new QHBoxLayout;
    lastLayout->addWidget ( new QLabel(tr("Last")) );
    lastEdit = new QLineEdit;
    lastEdit->setText("0");
    lastLayout->addWidget ( lastEdit );
    layout->addLayout(lastLayout);

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");
    connect(okButton, SIGNAL(clicked()), this, SLOT(defineVariable()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    setLayout(layout);
}

QSize DefineVariableDialog::sizeHint() const
{
    return QSize(200,200);
}

void DefineVariableDialog::defineVariable()
{
    result << nameEdit->text() << addressEdit->text()
           << formatCombo->currentText() << sizeCombo->currentText()
           << firstEdit->text() << lastEdit->text();
    accept();
}

ArrayBoundsDialog::ArrayBoundsDialog()
: QDialog()
{
    setObjectName("Set Array Bounds");
    setWindowTitle("Set Array Bounds");
    //setFrameStyle ( QFrame::Panel | QFrame::Raised );
    //setLineWidth(4);
    setModal(true);
    setWindowFlags(Qt::Dialog|Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10,10,10,10);

    QHBoxLayout *firstLayout = new QHBoxLayout;
    firstLayout->addWidget ( new QLabel(tr("First index")) );
    firstSpin = new QSpinBox;
    firstLayout->addWidget ( firstSpin );
    layout->addLayout(firstLayout);

    QHBoxLayout *lastLayout = new QHBoxLayout;
    lastLayout->addWidget ( new QLabel(tr("Last index")) );
    lastSpin = new QSpinBox;
    lastLayout->addWidget ( lastSpin );
    layout->addLayout(lastLayout);

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");
    connect(okButton, SIGNAL(clicked()), this, SLOT(setArrayBounds()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    setToolTip(tr("A pointer can point to an object or an array.\n") +
               tr("To point to an object set first and last to 0.") );
    setLayout(layout);
}

void ArrayBoundsDialog::setMax(int max)
{
    firstSpin->setMaximum(max);
    lastSpin->setMaximum(max);
}

void ArrayBoundsDialog::setMin(int min)
{
    firstSpin->setMinimum(min);
    lastSpin->setMinimum(min);
}

void ArrayBoundsDialog::setArrayBounds()
{
    min = firstSpin->value();
    max = lastSpin->value();
    if ( min > max ) {
        QMessageBox::warning(this,tr("Error"),
                     tr("The first index can't be\n greater than the last."),
                     QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    accept();
}

QSize ArrayBoundsDialog::sizeHint() const
{
    return QSize(200,200);
}

