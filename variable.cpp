#include "variable.h"
#include <QLabel>
#include <QMessageBox>

static bool arrayCheckedLastTime=true;

DefineVariableDialog::DefineVariableDialog()
    : QDialog()
{
    setObjectName("Define Variable");
    setWindowTitle(tr("Define Variable"));
    //setFrameStyle ( QFrame::Panel | QFrame::Raised );
    //setLineWidth(4);
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    layout = new QGridLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    layout->addWidget(new QLabel(tr("Name")), 0, 0);
    nameEdit = new QLineEdit;
    layout->addWidget(nameEdit, 0, 1);

    layout->addWidget(new QLabel(tr("Address")), 1, 0);
    addressEdit = new QLineEdit;
    layout->addWidget(addressEdit, 1, 1);

    QStringList types;
    types << "char" << "unsigned char" << "signed char" << "short"
        << "unsigned short" << "int" << "unsigned int"
#ifdef Q_OS_WIN32
        << "long long" << "unsigned long long"
#else
        << "long" << "unsigned long"
#endif
        << "float" << "double" << "bool";
    layout->addWidget(new QLabel(tr("type")), 2, 0);
    typeCombo = new QComboBox;
    typeCombo->addItems(types);
    layout->addWidget(typeCombo, 2, 1);

    layout->addWidget(new QLabel(tr("Format")), 3, 0);
    formatCombo = new QComboBox;
    layout->addWidget(formatCombo, 3, 1);

    layout->addWidget(new QLabel(tr("array variable")), 4, 0);
    arrayCheck = new QCheckBox;
    layout->addWidget(arrayCheck, 4, 1);

    layout->addWidget(new QLabel(tr("First")), 5, 0);
    firstEdit = new QLineEdit;
    firstEdit->setText("0");
    layout->addWidget(firstEdit, 5, 1);

    layout->addWidget(new QLabel(tr("Last")), 6, 0);
    lastEdit = new QLineEdit;
    lastEdit->setText("0");
    layout->addWidget(lastEdit, 6, 1);

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton(tr("Cancel"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(defineVariable()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(typeCombo, SIGNAL(currentIndexChanged(QString)), this,
        SLOT(typeChanged(QString)));
    connect(arrayCheck,SIGNAL(stateChanged(int)),
        this, SLOT(checkChanged(int)) );

    layout->addWidget(okButton, 7, 0);
    layout->addWidget(cancelButton, 7, 1);

    setLayout(layout);
    checkChanged(0);
    typeCombo->setCurrentIndex(5);
}

void DefineVariableDialog::checkChanged(int state)
{
    firstEdit->setEnabled(state ? true : false);
    lastEdit->setEnabled(state ? true : false);
    if (state == 0) {
        firstEdit->setText("0");
        lastEdit->setText("0");
    }
}

void DefineVariableDialog::typeChanged(QString type)
{
    QStringList formats;
    if (type.indexOf("char") >= 0) {
        formats << "character" << "hexadecimal" << "decimal" << "binary"
            << "string";
    } else if (type == "bool") {
        formats << "boolean" << "hexadecimal" << "binary";
    } else if (type == "float" || type == "double") {
        formats << "floating point" << "hexadecimal" << "binary" << "binary fp"
            << "fields";
    } else {
        formats << "hexadecimal" << "decimal" << "binary";
    }
    formatCombo->clear();
    formatCombo->addItems(formats);
    formatCombo->setCurrentIndex(0);
}

QSize DefineVariableDialog::sizeHint() const
{
    return QSize(200, 200);
}

void DefineVariableDialog::defineVariable()
{
    QString type = typeCombo->currentText();
    //if ( arrayCheck->isChecked() ) type = type + " *";
    result << nameEdit->text() << addressEdit->text() << type
        << formatCombo->currentText() << firstEdit->text() << lastEdit->text();
    accept();
}

ArrayBoundsDialog::ArrayBoundsDialog()
    : QDialog()
{
    setObjectName("Set Array Bounds");
    setWindowTitle(tr("Set Array Bounds"));
    //setFrameStyle ( QFrame::Panel | QFrame::Raised );
    //setLineWidth(4);
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout *checkLayout = new QHBoxLayout;
    checkLayout->addWidget(new QLabel(tr("array variable")));
    arrayCheck = new QCheckBox;
    arrayCheck->setChecked(arrayCheckedLastTime);
    checkLayout->addWidget(arrayCheck);
    layout->addLayout(checkLayout);

    QHBoxLayout *nLayout = new QHBoxLayout;
    nLayout->addWidget(new QLabel(tr("Number of elements")));
    nSpin = new QSpinBox;
    nLayout->addWidget(nSpin);
    layout->addLayout(nLayout);

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton(tr("Cancel"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(setArrayBounds()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(arrayCheck,SIGNAL(stateChanged(int)),
        this, SLOT(checkChanged(int)) );

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    setToolTip(
        tr("A pointer can point to an object or an array.\n")
            + tr("To point to an object set first and last to 0."));
    setLayout(layout);
}

void ArrayBoundsDialog::checkChanged(int state)
{
    nSpin->setEnabled(state ? true : false);
    arrayCheckedLastTime = state;
    if (state == 0) {
        nSpin->setValue(0);
    }
}

void ArrayBoundsDialog::setMax(int max)
{
    nSpin->setMaximum(max);
}

void ArrayBoundsDialog::setMin(int min)
{
    nSpin->setMinimum(min);
}

void ArrayBoundsDialog::setArrayBounds()
{
    n = nSpin->value();
    accept();
}

QSize ArrayBoundsDialog::sizeHint() const
{
    return QSize(200, 200);
}

