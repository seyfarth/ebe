#include "binarybitpanel.h"
#include "validators.h"
#include "settings.h"
#include <QLabel>

BinaryBitPanel::BinaryBitPanel(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10, 10, 10, 10);

    operatorCombo = new QComboBox(this);

    QStringList ops;
    ops << tr("|| Boolean or") << tr("| Bitwise or") << tr("&& Boolean and")
        << tr("& Bitwise and") << tr("^  Exclusive or") << tr("<< Shift left")
        << tr(">> Shift right") << tr(">> Signed shift right")
        << tr("@< Rotate left") << tr("@> Rotate right");
    operatorCombo->addItems(ops);

    QHBoxLayout *opLayout = new QHBoxLayout;
    opLayout->addWidget(new QLabel("Operator "));
    opLayout->addWidget(operatorCombo);
    opLayout->addStretch();

    layout->addLayout(opLayout);

    table = new QTableWidget;

    //   input  do-it  format result comment
    table->setColumnCount(4);
    table->setRowCount(2);
    QStringList headers;
    headers << tr("Input") << tr("Operator") << tr("Result") << tr("Comment");
    table->setHorizontalHeaderLabels(headers);

    //number = new BinaryNumber;
    //number->setBits(32354,16);

    //layout->addWidget(number);

    input1 = new BinaryNumber;
    input1Edit = new IntegerEdit;

    input2 = new BinaryNumber;
    input2Edit = new IntegerEdit;

    fontSize = ebe["font_size"].toInt();
    table->setCellWidget(0, 0, input1Edit);
    table->setCellWidget(0, 2, input1);
    table->setCellWidget(1, 0, input2Edit);
    table->setCellWidget(1, 2, input2);
    table->setColumnWidth(0, 8 * fontSize);
    table->setColumnWidth(1, 8 * fontSize);
    table->setColumnWidth(2, 16 * fontSize + 5);
    table->setColumnWidth(3, 16 * fontSize);

    layout->addWidget(table);

    setLayout(layout);

    connect(operatorCombo, SIGNAL(activated(QString)), this,
        SLOT(selectOperator(QString)));

    selectOperator(tr("|| Boolean or"));

}

void BinaryBitPanel::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
}

void BinaryBitPanel::selectOperator(QString o)
{
    input1->clear();
    input2->clear();
    if (o.left(2) == "||") {
        op = "||";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(orStep1()));
    } else if (o.left(1) == "|") {
        op = "|";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(bitwiseOrStep1()));
    } else if (o.left(2) == "&&") {
        op = "&&&&";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(andStep1()));
    } else if (o.left(1) == "&") {
        op = "&&";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(bitwiseAndStep1()));
    } else if (o.left(1) == "^") {
        op = "^";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(xorStep1()));
    } else if (o.indexOf("Signed") >= 0) {
        op = ">> signed";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(signedShiftRightStep1()));
    } else if (o.left(2) == "<<") {
        op = "<<";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(shiftLeftStep1()));
    } else if (o.left(2) == ">>") {
        op = ">>";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(shiftRightStep1()));
    } else if (o.left(2) == "@<") {
        op = "@< (not in C++)";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(rotateLeftStep1()));
    } else if (o.left(2) == "@>") {
        op = "@> (not in C++)";
        doit = new QPushButton(op);
        table->setRowCount(2);
        table->setCellWidget(0, 1, doit);
        table->setCellWidget(0, 3, new QLabel(""));
        table->setCellWidget(1, 3, new QLabel(""));
        connect(doit, SIGNAL(clicked()), this, SLOT(rotateRightStep1()));
    } else {
        qDebug() << "Unknown operator:" << o;
    }
}

void BinaryBitPanel::orStep1()
{
    value1 = input1Edit->value();
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(orStep2()));
}

void BinaryBitPanel::orStep2()
{
    output = new BinaryNumber;
    output->setBits(value1 || value2, 16, 8);
    table->setCellWidget(2, 2, output);
    table->setCellWidget(2, 3, new QLabel(tr("Result as a binary bool")));
    table->setCellWidget(2, 1, new QLabel(""));
}

void BinaryBitPanel::bitwiseOrStep1()
{
    value1 = input1Edit->value();
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(bitwiseOrStep2()));
}

void BinaryBitPanel::bitwiseOrStep2()
{
    output = new BinaryNumber;
    output->setBits(value1 | value2, 16);
    table->setCellWidget(2, 2, output);
    table->setCellWidget(2, 3, new QLabel(tr("Result")));
    table->setCellWidget(2, 1, new QLabel(""));
}

void BinaryBitPanel::andStep1()
{
    value1 = input1Edit->value();
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(andStep2()));
}

void BinaryBitPanel::andStep2()
{
    output = new BinaryNumber;
    output->setBits(value1 && value2, 16, 8);
    table->setCellWidget(2, 2, output);
    table->setCellWidget(2, 3, new QLabel(tr("Result as a binary bool")));
    table->setCellWidget(2, 1, new QLabel(""));
}

void BinaryBitPanel::bitwiseAndStep1()
{
    value1 = input1Edit->value();
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(bitwiseAndStep2()));
}

void BinaryBitPanel::bitwiseAndStep2()
{
    output = new BinaryNumber;
    output->setBits(value1 & value2, 16);
    table->setCellWidget(2, 2, output);
    table->setCellWidget(2, 3, new QLabel(tr("Result")));
    table->setCellWidget(2, 1, new QLabel(""));
}

void BinaryBitPanel::xorStep1()
{
    value1 = input1Edit->value();
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(xorStep2()));
}

void BinaryBitPanel::xorStep2()
{
    output = new BinaryNumber;
    output->setBits(value1 ^ value2, 16);
    table->setCellWidget(2, 2, output);
    table->setCellWidget(2, 3, new QLabel(tr("Result")));
    table->setCellWidget(2, 1, new QLabel(""));
}

void BinaryBitPanel::shiftLeftStep1()
{
    value1 = input1Edit->value();
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);
    if (value2 > 16) value2 = 16;
    count = 1;

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(shiftLeftStep2()));
}

void BinaryBitPanel::shiftLeftStep2()
{
    output = new BinaryNumber;
    output->setBits(value1 << count, 16);
    table->setRowCount(count + 2);
    table->setCellWidget(count + 1, 2, output);
    table->setCellWidget(count + 1, 1, new QLabel(""));
    if (count == value2) {
        table->setCellWidget(count + 1, 3, new QLabel(tr("Result")));
    } else {
        doit = new QPushButton(op);
        table->setRowCount(count + 3);
        table->setCellWidget(count + 2, 1, doit);
        connect(doit, SIGNAL(clicked()), this, SLOT(shiftLeftStep2()));
    }
    count++;
}

void BinaryBitPanel::shiftRightStep1()
{
    value1 = input1Edit->value() & 0xffff;
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);
    if (value2 > 16) value2 = 16;
    count = 1;

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(shiftRightStep2()));
}

void BinaryBitPanel::shiftRightStep2()
{
    output = new BinaryNumber;
    output->setBits((unsigned)value1 >> count, 16);
    table->setRowCount(count + 2);
    table->setCellWidget(count + 1, 2, output);
    table->setCellWidget(count + 1, 1, new QLabel(""));
    if (count == value2) {
        table->setCellWidget(count + 1, 3, new QLabel(tr("Result")));
    } else {
        doit = new QPushButton(op);
        table->setRowCount(count + 3);
        table->setCellWidget(count + 2, 1, doit);
        connect(doit, SIGNAL(clicked()), this, SLOT(shiftRightStep2()));
    }
    count++;
}

void BinaryBitPanel::signedShiftRightStep1()
{
    value1 = input1Edit->value() & 0xffff;
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);
    if (value2 > 16) value2 = 16;
    count = 1;

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(signedShiftRightStep2()));
}

void BinaryBitPanel::signedShiftRightStep2()
{
    output = new BinaryNumber;
    output->setBits((short)value1 >> count, 16);
    table->setRowCount(count + 2);
    table->setCellWidget(count + 1, 2, output);
    table->setCellWidget(count + 1, 1, new QLabel(""));
    if (count == value2) {
        table->setCellWidget(count + 1, 3, new QLabel(tr("Result")));
    } else {
        doit = new QPushButton(op);
        table->setRowCount(count + 3);
        table->setCellWidget(count + 2, 1, doit);
        connect(doit, SIGNAL(clicked()), this, SLOT(signedShiftRightStep2()));
    }
    count++;
}

void BinaryBitPanel::rotateLeftStep1()
{
    value1 = input1Edit->value() & 0xffff;
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);
    if (value2 > 16) value2 = 16;
    count = 1;

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(rotateLeftStep2()));
}

void BinaryBitPanel::rotateLeftStep2()
{
    short left, right;
    output = new BinaryNumber;
    left = (unsigned)value1 << count;
    right = (unsigned)value1 >> (16 - count);
    output->setBits(left | right, 16);
    table->setRowCount(count + 2);
    table->setCellWidget(count + 1, 2, output);
    table->setCellWidget(count + 1, 1, new QLabel(""));
    if (count == value2) {
        table->setCellWidget(count + 1, 3, new QLabel(tr("Result")));
    } else {
        doit = new QPushButton(op);
        table->setRowCount(count + 3);
        table->setCellWidget(count + 2, 1, doit);
        connect(doit, SIGNAL(clicked()), this, SLOT(rotateLeftStep2()));
    }
    count++;
}

void BinaryBitPanel::rotateRightStep1()
{
    value1 = input1Edit->value() & 0xffff;
    input1->setBits(value1, 16);

    value2 = input2Edit->value();
    input2->setBits(value2, 16);
    if (value2 > 16) value2 = 16;
    count = 1;

    table->setCellWidget(0, 3, new QLabel(tr("Values converted to binary")));
    table->setCellWidget(0, 1, new QLabel(""));
    doit = new QPushButton(op);
    table->setRowCount(3);
    table->setCellWidget(2, 1, doit);
    connect(doit, SIGNAL(clicked()), this, SLOT(rotateRightStep2()));
}

void BinaryBitPanel::rotateRightStep2()
{
    short left, right;
    output = new BinaryNumber;
    left = (unsigned)value1 >> count;
    right = (unsigned)value1 << (16 - count);
    output->setBits(left | right, 16);
    table->setRowCount(count + 2);
    table->setCellWidget(count + 1, 2, output);
    table->setCellWidget(count + 1, 1, new QLabel(""));
    if (count == value2) {
        table->setCellWidget(count + 1, 3, new QLabel(tr("Result")));
    } else {
        doit = new QPushButton(op);
        table->setRowCount(count + 3);
        table->setCellWidget(count + 2, 1, doit);
        connect(doit, SIGNAL(clicked()), this, SLOT(rotateRightStep2()));
    }
    count++;
}

