#include "intconvert.h"
#include "settings.h"

#include <QLabel>
#include <QMessageBox>

IntConvert::IntConvert(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10, 10, 10, 10);

    convertCombo = new QComboBox(this);

    QStringList ops;
    ops << tr("Decimal to Binary") << tr("Decimal to Hex")
        << tr("Binary to Decimal") << tr("Hex to Decimal")
        << tr("Binary to Hex") << tr("Hex to Binary");

    convertCombo->addItems(ops);

    QHBoxLayout *opLayout = new QHBoxLayout;
    opLayout->addWidget(new QLabel(tr("Operator ")));
    opLayout->addWidget(convertCombo);
    opLayout->addStretch();

    layout->addLayout(opLayout);

    table = new QTableWidget;

    //   input  do-it  format result comment
    table->setColumnCount(7);
    table->setRowCount(1);

    fontSize = ebe["font_size"].toInt();
    table->setColumnWidth(0, 8 * fontSize);
    table->setColumnWidth(1, 8 * fontSize);
    table->setColumnWidth(2, 4 * fontSize);
    table->setColumnWidth(3, 4 * fontSize);
    table->setColumnWidth(4, 4 * fontSize);
    table->setColumnWidth(5, 16 * fontSize + 5);
    table->setColumnWidth(6, 16 * fontSize);

    layout->addWidget(table);

    setLayout(layout);

    doit = new QPushButton(tr("to binary"));

    connect(convertCombo, SIGNAL(activated(QString)), this,
        SLOT(selectOperator(QString)));

    selectOperator(tr("Decimal to Binary"));

}

void IntConvert::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    for (int row = 0; row < table->rowCount(); row++) {
        table->setRowHeight(row, fontHeight + 4);
    }
}

void IntConvert::selectOperator(QString o)
{
    QStringList headers;

    if (o == tr("Decimal to Binary")) {
        op = o;
        table->clear();
        table->setColumnCount(7);
        table->setRowCount(1);
        inputEdit = new IntegerEdit;
        table->setCellWidget(0, 0, inputEdit);
        headers << tr("Input") << tr("Conversion") << " n " << "n / 2"
            << "n % 2" << tr("Result") << tr("Comment");
        table->setHorizontalHeaderLabels(headers);
        doit = new QPushButton(tr("to binary"));
        table->setCellWidget(0, 1, doit);
        table->setColumnWidth(0, 8 * fontSize);
        table->setColumnWidth(1, 8 * fontSize);
        table->setColumnWidth(2, 4 * fontSize);
        table->setColumnWidth(3, 4 * fontSize);
        table->setColumnWidth(4, 4 * fontSize);
        table->setColumnWidth(5, 16 * fontSize + 5);
        table->setColumnWidth(6, 16 * fontSize);
        connect(doit, SIGNAL(clicked()), this, SLOT(decimalToBinary1()));
    } else if (o == tr("Decimal to Hex")) {
        op = o;
        table->clear();
        table->setColumnCount(7);
        table->setRowCount(1);
        inputEdit = new IntegerEdit;
        table->setCellWidget(0, 0, inputEdit);
        headers << tr("Input") << tr("Conversion") << " n " << "n / 16"
            << "n % 16" << tr("Result") << tr("Comment");
        table->setHorizontalHeaderLabels(headers);
        table->setColumnWidth(0, 8 * fontSize);
        table->setColumnWidth(1, 8 * fontSize);
        table->setColumnWidth(2, 4 * fontSize);
        table->setColumnWidth(3, 4 * fontSize);
        table->setColumnWidth(4, 4 * fontSize);
        table->setColumnWidth(5, 4 * fontSize + 5);
        table->setColumnWidth(6, 16 * fontSize);
        doit = new QPushButton("to hex");
        table->setCellWidget(0, 1, doit);
        connect(doit, SIGNAL(clicked()), this, SLOT(decimalToHex1()));
    } else if (o == tr("Binary to Decimal")) {
        op = o;
        table->clear();
        table->setColumnCount(7);
        table->setRowCount(1);
        inputEdit = new IntegerEdit;
        table->setCellWidget(0, 0, inputEdit);
        headers << tr("Input") << tr("Conversion") << tr("number") << "bit"
            << "2**bit" << tr("Result") << tr("Comment");
        table->setHorizontalHeaderLabels(headers);
        doit = new QPushButton(tr("to decimal"));
        table->setCellWidget(0, 1, doit);
        table->setColumnWidth(0, 8 * fontSize);
        table->setColumnWidth(1, 8 * fontSize);
        table->setColumnWidth(2, 16 * fontSize + 5);
        table->setColumnWidth(3, 4 * fontSize);
        table->setColumnWidth(4, 6 * fontSize);
        table->setColumnWidth(5, 6 * fontSize);
        table->setColumnWidth(6, 16 * fontSize);
        connect(doit, SIGNAL(clicked()), this, SLOT(binaryToDecimal1()));
    } else if (o == tr("Hex to Decimal")) {
        op = o;
        table->clear();
        table->setColumnCount(8);
        table->setRowCount(1);
        inputEdit = new IntegerEdit;
        table->setCellWidget(0, 0, inputEdit);
        headers << tr("Input") << tr("Conversion") << tr("number")
            << tr("nibble") << tr("2**nibble") << tr("nibble value")
            << tr("Result") << tr("Comment");
        table->setHorizontalHeaderLabels(headers);
        doit = new QPushButton(tr("to decimal"));
        table->setCellWidget(0, 1, doit);
        table->setColumnWidth(0, 8 * fontSize);
        table->setColumnWidth(1, 8 * fontSize);
        table->setColumnWidth(2, 4 * fontSize + 5);
        table->setColumnWidth(3, 6 * fontSize);
        table->setColumnWidth(4, 8 * fontSize);
        table->setColumnWidth(5, 12 * fontSize);
        table->setColumnWidth(6, 6 * fontSize);
        table->setColumnWidth(7, 16 * fontSize);
        connect(doit, SIGNAL(clicked()), this, SLOT(hexToDecimal1()));
    } else if (o == tr("Binary to Hex")) {
        op = o;
        table->clear();
        table->setColumnCount(6);
        table->setRowCount(1);
        inputEdit = new IntegerEdit;
        table->setCellWidget(0, 0, inputEdit);
        headers << tr("Input") << tr("Conversion") << tr("number")
            << tr("nibble") << tr("Result") << tr("Comment");
        table->setHorizontalHeaderLabels(headers);
        doit = new QPushButton("to hex");
        table->setCellWidget(0, 1, doit);
        table->setColumnWidth(0, 8 * fontSize);
        table->setColumnWidth(1, 8 * fontSize);
        table->setColumnWidth(2, 16 * fontSize + 5);
        table->setColumnWidth(3, 4 * fontSize);
        table->setColumnWidth(4, 5 * fontSize);
        table->setColumnWidth(5, 16 * fontSize);
        connect(doit, SIGNAL(clicked()), this, SLOT(binaryToHex1()));
    } else if (o == tr("Hex to Binary")) {
        op = o;
        table->clear();
        table->setColumnCount(6);
        table->setRowCount(1);
        inputEdit = new IntegerEdit;
        table->setCellWidget(0, 0, inputEdit);
        headers << tr("Input") << tr("Conversion") << tr("number")
            << tr("nibble") << tr("Result") << tr("Comment");
        table->setHorizontalHeaderLabels(headers);
        doit = new QPushButton(tr("to binary"));
        table->setCellWidget(0, 1, doit);
        table->setColumnWidth(0, 8 * fontSize);
        table->setColumnWidth(1, 8 * fontSize);
        table->setColumnWidth(2, 5 * fontSize);
        table->setColumnWidth(3, 4 * fontSize);
        table->setColumnWidth(4, 16 * fontSize + 5);
        table->setColumnWidth(5, 16 * fontSize);
        connect(doit, SIGNAL(clicked()), this, SLOT(hexToBinary1()));
    } else {
        qDebug() << tr("Unknown operator:") << o;
    }
}

void IntConvert::decimalToBinary1()
{
    QLabel *label;
    if (!inputEdit->isValid()) {
        QMessageBox::warning(this, tr("Warning"),
            tr("The input value is not complete."), QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }
    value = inputEdit->value();
    value = value & 0xffff;
    computedValue = 0;
    bit = 0;
    table->setCellWidget(0, 6,
        new QLabel(tr("Value & 0xffff copied into column 3")));
    label = new QLabel(QString("%1").arg(value));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(0, 2, label);
    connect(doit, SIGNAL(clicked()), this, SLOT(decimalToBinary2()));
}

void IntConvert::decimalToBinary2()
{
    int n = value % 2;
    QLabel *label;

    output = new BinaryNumber;
    computedValue = (n << bit) + computedValue;
    output->setBits(computedValue, 16, bit + 1);

    table->setCellWidget(bit, 1, new QLabel(""));
    doit->setText("");
    doit->setDisabled(true);
    label = new QLabel(QString("%1").arg(value / 2));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(bit, 3, label);
    label = new QLabel(QString("%1").arg(value % 2));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(bit, 4, label);
    table->setCellWidget(bit, 5, output);
    table->setCellWidget(bit, 6, new QLabel(tr("divide by 2")));

    value = value >> 1;
    if (value == 0) return;
    bit++;
    table->setRowCount(bit + 1);
    doit = new QPushButton(tr("to binary"));
    table->setCellWidget(bit, 1, doit);
    label = new QLabel(QString("%1").arg(value));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(bit, 0, new QLabel(""));
    table->setCellWidget(bit, 2, label);
    table->setCellWidget(bit, 3, new QLabel(""));
    table->setCellWidget(bit, 4, new QLabel(""));
    table->setCellWidget(bit, 5, new QLabel(""));
    connect(doit, SIGNAL(clicked()), this, SLOT(decimalToBinary2()));
    table->setCellWidget(bit, 6, new QLabel(tr("moved n/2 to n in new row")));
}

void IntConvert::decimalToHex1()
{
    QLabel *label;
    if (!inputEdit->isValid()) {
        QMessageBox::warning(this, tr("Warning"),
            tr("The input value is not complete."), QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }
    value = inputEdit->value();
    value = value & 0xffff;
    computedValue = 0;
    nibble = 0;
    table->setCellWidget(0, 6,
        new QLabel(tr("Value & 0xffff copied into column 3")));
    label = new QLabel(QString("%1").arg(value));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(0, 2, label);
    connect(doit, SIGNAL(clicked()), this, SLOT(decimalToHex2()));
}

void IntConvert::decimalToHex2()
{
    int n = value % 16;
    QLabel *label;

    output = new BinaryNumber;
    computedValue = (n << (nibble * 4)) + computedValue;
    output->setNibbles(computedValue, 4, nibble + 1);

    table->setCellWidget(nibble, 1, new QLabel(""));
    doit->setText("");
    doit->setDisabled(true);
    label = new QLabel(QString("%1").arg(value / 16));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(nibble, 3, label);
    label = new QLabel(QString("%1").arg(value % 16));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(nibble, 4, label);
    table->setCellWidget(nibble, 5, output);
    table->setCellWidget(nibble, 6, new QLabel(tr("divide by 16")));

    value = value >> 4;
    if (value == 0) return;
    nibble++;
    table->setRowCount(nibble + 1);
    doit = new QPushButton(tr("to hex"));
    table->setCellWidget(nibble, 1, doit);
    label = new QLabel(QString("%1").arg(value));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(nibble, 0, new QLabel(""));
    table->setCellWidget(nibble, 2, label);
    table->setCellWidget(nibble, 3, new QLabel(""));
    table->setCellWidget(nibble, 4, new QLabel(""));
    table->setCellWidget(nibble, 5, new QLabel(""));
    connect(doit, SIGNAL(clicked()), this, SLOT(decimalToHex2()));
    table->setCellWidget(nibble, 6,
        new QLabel(tr("moved n/16 to n in new row")));
}

void IntConvert::binaryToDecimal1()
{
    if (!inputEdit->isValid()) {
        QMessageBox::warning(this, tr("Warning"),
            tr("The input value is not complete."), QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }
    value = inputEdit->value();
    value = value & 0xffff;
    computedValue = 0;
    bit = 0;
    row = 0;
    binaryToDecimal2();
}

void IntConvert::binaryToDecimal2()
{
    int n = value % 2;
    //qDebug() << "value" << value << ",  n" << n;
    while (n == 0 && bit < 16) {
        bit++;
        value = value >> 1;
        n = value % 2;
    }
    if (n == 0) {
        table->setCellWidget(bit, 1, new QLabel(""));
        return;
    }
    //qDebug() << "value" << value << ",  n" << n;

    QLabel *label;

    //headers << "Input" << "Conversion" << "number" << "bit"
    //        << "2**bit" << "Sum" << "Comment";
    computedValue = (1 << bit) + computedValue;
    table->setCellWidget(row, 1, new QLabel(""));
    output = new BinaryNumber;
    output->setBits(inputEdit->value() & 0xffff, 16);
    output->setHighlight(bit);
    table->setCellWidget(row, 2, output);
    label = new QLabel(QString("%1").arg(bit));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 3, label);
    label = new QLabel(QString("%1").arg(1 << bit));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 4, label);
    label = new QLabel(QString("%1").arg(computedValue));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 5, label);
    table->setCellWidget(row, 6,
        new QLabel(
            QString(tr("Result is value for rightmost %1 bits")).arg(bit + 1)));

    value = value >> 1;
    bit++;

    if (value == 0) {
        table->setCellWidget(row, 6, new QLabel(tr("Done")));
        return;
    }

    row++;
    table->setRowCount(row + 1);
    doit = new QPushButton(tr("to decimal"));
    table->setCellWidget(row, 0, new QLabel(""));
    table->setCellWidget(row, 1, doit);
    table->setCellWidget(row, 2, new QLabel(""));
    table->setCellWidget(row, 3, new QLabel(""));
    table->setCellWidget(row, 4, new QLabel(""));
    table->setCellWidget(row, 5, new QLabel(""));
    table->setCellWidget(row, 6, new QLabel(""));
    connect(doit, SIGNAL(clicked()), this, SLOT(binaryToDecimal2()));
}

void IntConvert::hexToDecimal1()
{
    if (!inputEdit->isValid()) {
        QMessageBox::warning(this, tr("Warning"),
            tr("The input value is not complete."), QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }
    value = inputEdit->value();
    value = value & 0xffff;
    computedValue = 0;
    nibble = 0;
    row = 0;
    hexToDecimal2();
}

void IntConvert::hexToDecimal2()
{
    int n = value % 16;
    //qDebug() << "value" << value << ",  n" << n;
    while (n == 0 && nibble < 4) {
        nibble++;
        value = value >> 4;
        n = value % 16;
    }
    if (n == 0) {
        table->setCellWidget(bit, 1, new QLabel(""));
        return;
    }
    //qDebug() << "value" << value << ",  n" << n;

    QLabel *label;

    computedValue = n * (1 << (nibble * 4)) + computedValue;
    table->setCellWidget(row, 1, new QLabel(""));
    output = new BinaryNumber;
    output->setNibbles(inputEdit->value() & 0xffff, 4);
    output->setHighlight(nibble);
    table->setCellWidget(row, 2, output);
    label = new QLabel(QString("%1").arg(nibble));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 3, label);
    label = new QLabel(QString("%1").arg(1 << (nibble * 4)));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 4, label);
    label = new QLabel(QString("%1").arg(n * (1 << (nibble * 4))));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 5, label);
    label = new QLabel(QString("%1").arg(computedValue));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 6, label);
    table->setCellWidget(row, 7,
        new QLabel(
            QString(tr("Result is value for rightmost %1 nibbles")).arg(
                nibble + 1)));

    value = value >> 4;
    nibble++;

    if (value == 0) {
        table->setCellWidget(row, 7, new QLabel(tr("Done")));
        return;
    }

    row++;
    table->setRowCount(row + 1);
    doit = new QPushButton(tr("to decimal"));
    table->setCellWidget(row, 0, new QLabel(""));
    table->setCellWidget(row, 1, doit);
    table->setCellWidget(row, 2, new QLabel(""));
    table->setCellWidget(row, 3, new QLabel(""));
    table->setCellWidget(row, 4, new QLabel(""));
    table->setCellWidget(row, 5, new QLabel(""));
    table->setCellWidget(row, 6, new QLabel(""));
    table->setCellWidget(row, 7, new QLabel(""));
    connect(doit, SIGNAL(clicked()), this, SLOT(hexToDecimal2()));
}

void IntConvert::binaryToHex1()
{
    if (!inputEdit->isValid()) {
        QMessageBox::warning(this, tr("Warning"),
            tr("The input value is not complete."), QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }
    value = inputEdit->value();
    value = value & 0xffff;
    computedValue = 0;
    nibble = 0;
    row = 0;
    binaryToHex2();
}

void IntConvert::binaryToHex2()
{
    int n = value % 16;
    //qDebug() << "value" << value << ",  n" << n;

    QLabel *label;

    computedValue += n * (1 << (nibble * 4));
    table->setCellWidget(row, 1, new QLabel(""));
    output = new BinaryNumber;
    output->setBits(inputEdit->value() & 0xffff, 16);
    output->setHighlight(nibble * 4, nibble * 4 + 3);
    table->setCellWidget(row, 2, output);
    label = new QLabel(QString("%1").arg(nibble));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 3, label);
    output = new BinaryNumber;
    output->setNibbles(computedValue, 4, nibble + 1);
    table->setCellWidget(row, 4, output);
    QString t;
    t.sprintf("%d == 0x%X", n, n);
    table->setCellWidget(row, 5, new QLabel(t));

    value = value >> 4;
    nibble++;

    if (value == 0) {
        return;
    }

    row++;
    table->setRowCount(row + 1);
    doit = new QPushButton(tr("to hex"));
    table->setCellWidget(row, 0, new QLabel(""));
    table->setCellWidget(row, 1, doit);
    table->setCellWidget(row, 2, new QLabel(""));
    table->setCellWidget(row, 3, new QLabel(""));
    table->setCellWidget(row, 4, new QLabel(""));
    table->setCellWidget(row, 5, new QLabel(""));
    connect(doit, SIGNAL(clicked()), this, SLOT(binaryToHex2()));
}

void IntConvert::hexToBinary1()
{
    if (!inputEdit->isValid()) {
        QMessageBox::warning(this, tr("Warning"),
            tr("The input value is not complete."), QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }
    value = inputEdit->value();
    value = value & 0xffff;
    computedValue = 0;
    nibble = 0;
    row = 0;
    hexToBinary2();
}

static char *bits[] = { (char *)"0000", (char *)"0001", (char *)"0010",
    (char *)"0011", (char *)"0100", (char *)"0101", (char *)"0110",
    (char *)"0111", (char *)"1000", (char *)"1001", (char *)"1010",
    (char *)"1011", (char *)"1100", (char *)"1101", (char *)"1110",
    (char *)"1111" };

void IntConvert::hexToBinary2()
{
    int n = value % 16;
    //qDebug() << "value" << value << ",  n" << n;

    QLabel *label;

    computedValue += n * (1 << (nibble * 4));
    table->setCellWidget(row, 1, new QLabel(""));
    output = new BinaryNumber;
    output->setNibbles(inputEdit->value() & 0xffff, 4);
    output->setHighlight(nibble);
    table->setCellWidget(row, 2, output);
    label = new QLabel(QString("%1").arg(nibble));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(row, 3, label);
    output = new BinaryNumber;
    output->setBits(computedValue, 16, (nibble + 1) * 4);
    table->setCellWidget(row, 4, output);
    QString t;
    t.sprintf("0x%X == 0b%s", n, bits[n]);
    table->setCellWidget(row, 5, new QLabel(t));

    value = value >> 4;
    nibble++;

    if (value == 0) {
        return;
    }

    row++;
    table->setRowCount(row + 1);
    doit = new QPushButton(tr("to binary"));
    table->setCellWidget(row, 0, new QLabel(""));
    table->setCellWidget(row, 1, doit);
    table->setCellWidget(row, 2, new QLabel(""));
    table->setCellWidget(row, 3, new QLabel(""));
    table->setCellWidget(row, 4, new QLabel(""));
    table->setCellWidget(row, 5, new QLabel(""));
    connect(doit, SIGNAL(clicked()), this, SLOT(hexToBinary2()));
}
