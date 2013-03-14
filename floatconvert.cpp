#include "floatconvert.h"
#include "settings.h"

FloatConvert::FloatConvert(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10,10,10,10);

    convertCombo = new QComboBox(this);

    QStringList ops;
    ops << "Hex to Float" << "Float to Hex";

    convertCombo->addItems(ops);

    QHBoxLayout *opLayout = new QHBoxLayout;
    opLayout->addWidget(new QLabel("Operator "));
    opLayout->addWidget(convertCombo);
    opLayout->addStretch();

    layout->addLayout ( opLayout );

    table = new QTableWidget;

    //   input  do-it  format result comment
    table->setColumnCount(7);
    table->setRowCount(1);

    fontSize = ebe["font_size"].toInt();
    table->setColumnWidth(0,8*fontSize);
    table->setColumnWidth(1,8*fontSize);
    table->setColumnWidth(2,4*fontSize);
    table->setColumnWidth(3,4*fontSize);
    table->setColumnWidth(4,4*fontSize);
    table->setColumnWidth(5,16*fontSize+5);
    table->setColumnWidth(6,16*fontSize);
    
    layout->addWidget(table);

    setLayout(layout);

    doit = new QPushButton("to binary");

    connect ( convertCombo, SIGNAL(activated(QString)),
              this, SLOT(selectOperator(QString)) );

    selectOperator("Hex to Float");

}

void FloatConvert::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth  = width;
}

void FloatConvert::selectOperator(QString o)
{
    QStringList headers;

    if ( o == "Hex to Float" ) {
        op = o;
        table->clear();
        table->setColumnCount(3);
        table->setRowCount(1);
        inputEdit = new FloatEdit;
        table->setCellWidget(0,0,inputEdit);
        headers << "" << "" << "Comment";
        table->setHorizontalHeaderLabels(headers);
        table->setVerticalHeaderItem(0,new QTableWidgetItem("Input"));
        doit = new QPushButton("to float");
        table->setCellWidget(0,1,doit);
        table->setColumnWidth(0,32*fontSize+5);
        table->setColumnWidth(1,6*fontSize);
        table->setColumnWidth(2,28*fontSize);
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat1()) );
    } else {
        qDebug() << "Unknown operator:" << o;
    }
}

void FloatConvert::hexToFloat1()
{
    if ( ! inputEdit->isValid() ) {
        QMessageBox::warning(this,tr("Warning"),
            tr("The input value is not complete."),
            QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    value = inputEdit->value() & 0xffffffff;
    computedValue = 0;
    bit = 0;
    row = 1;
    if ( value == 0 ) {
        toFloatState = Zero;
    } else {
        toFloatState = Bits;
    }
    hexToFloat2();
}

QString binary(int n, int bits )
{
    QString s;
    for ( int b = 0; b < bits; b++ ) {
        s = ((n >> b) & 1 ? "1" : "0") + s;
    }
    return s;
}

void FloatConvert::hexToFloat2()
{
    QLabel *label;
    QString bits;

    switch ( toFloatState ) {
    case Zero:
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        label = new QLabel("0.0");
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(row,0,label);
        label = new QLabel(" All bits are 0 so the number is 0");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Value"));
        break;

    case NegativeZero:
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        label = new QLabel("-0.0");
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(row,0,label);
        label = new QLabel(" Bits 0-30 are 0 so the number is -0");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Value"));
        break;

    case Bits:
        table->setRowCount(row+1);
        input = new BinaryNumber;
        input->setBits(value, 32, 32 );
        table->setCellWidget(row-1,1,new QLabel(""));
        table->setCellWidget(row,0,input);
        label = new QLabel(" Number converted to binary");
        table->setCellWidget(row,2,label);
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Bits"));
        row++;
        toFloatState = SignBit;
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case SignBit:
        input->setHighlight(31);
        //table->setCellWidget(1,0,input);
        table->setRowCount(row+1);
        sign = (value & 0x80000000) == 0 ? 0 : 1;
        table->setCellWidget(row-1,1,new QLabel(""));
        label = new QLabel(QString("%1").arg(sign));
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(row,0,label);
        if ( sign == 0 ) {
            label = new QLabel(" Sign bit (bit 31) is 0. The number is positive");
        } else {
            label = new QLabel(" Sign bit (bit 31) is 1. The number is negative");
        }
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Sign bit"));
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        row++;
        if ( (value & 0x7fffffff) == 0 ) {
            toFloatState = NegativeZero;
        } else {
            toFloatState = ExponentField;
        }
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case ExponentField:
        input->setHighlight(23,30);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        exponentField = (value >> 23) & 0xff;
        label = new QLabel(QString("%1").arg(exponentField));
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(row,0,label);
        label = new QLabel(" The exponent field is bits 23 - 30");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Exponent field"));
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        row++;
        if ( exponentField == 0 ) {
            toFloatState = Denormalized;
        } else if ( exponentField == 255 ) {
            if ( (value & 0x7fffff) == 0 ) {
                toFloatState = Infinity;
            } else {
                toFloatState = NaN;
            }
        } else {
            toFloatState = ExponentValue;
        }
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case ExponentValue:
        input->setHighlight(-1);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        exponentValue = exponentField - 127;
        label = new QLabel(QString("%1").arg(exponentValue));
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(row,0,label);
        label = new QLabel(" The exponent value is the field value - 127");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Exponent value"));
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        row++;
        toFloatState = FractionField;
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case FractionField:
        input->setHighlight(0,22);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        fractionField = value & 0x7fffff;
        output = new BinaryNumber;
        output->setBits ( fractionField, 24, 23 );
        table->setCellWidget(row,0,output);
        label = new QLabel(" The exponent field is the rightmost 23 bits");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Fraction field"));
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        row++;
        toFloatState = FractionValue;
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case FractionValue:
        input->setHighlight(-1);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        fractionValue = fractionField | 0x800000;
        output = new BinaryNumber;
        output->setBits ( fractionValue, 24 );
        table->setCellWidget(row,0,output);
        label = new QLabel(" The exponent value has an implicit 1 bit");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Fraction value"));
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        row++;
        toFloatState = BinaryValue;
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case BinaryValue:
        wholePart = fractionValue >> 23;
        fractionPart = fractionValue & 0x7fffff;
        bits = binary(fractionPart,23);
        label = new QLabel(
                QString(" %1.%2 * 2**%3").arg(wholePart).arg(bits).
                        arg(exponentValue));
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        table->setCellWidget(row,0,label);
        label = new QLabel(" Value as a binary number times 2 to a power");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Binary value"));
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        row++;
        bit = 23;
        result = 0.0;
        toFloatState = BitValue;
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case BitValue:
        while ( bit >= 0 && (fractionValue & (1 << bit)) == 0 ) bit--;
        if ( bit < 0 ) {
            toFloatState = Result;
            hexToFloat2();
            break;
        }
        output->setHighlight(bit);
        bitValue = pow ( 2.0, bit-23+exponentValue );
        result += bitValue;

        label = new QLabel(QString(" %1").arg(bitValue));
        label->setAlignment(Qt::AlignCenter);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        table->setCellWidget(row,0,label);
        label = new QLabel(QString(" Bit %1 value is 2**%2").arg(bit).
                           arg(bit-23+exponentValue));
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem(
               QString("Bit %1").arg(bit)));
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        row++;
        bit--;
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case Result:
        output->setHighlight(-1);
        if ( sign != 0 ) result = -result;
        label = new QLabel(QString(" %1").arg(result));
        label->setAlignment(Qt::AlignCenter);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        table->setCellWidget(row,0,label);
        if ( sign == 0 ) {
            label = new QLabel(QString(" Add the bit values"));
        } else {
            label = new QLabel(QString(" Add the bit values and multiply by -1"));
        }
        table->setCellWidget(row,2,label);
        break;

    case Denormalized:
        exponentValue = -126;
        fractionValue = value & 0x7fffff;
        bit = 23;
        result = 0.0;
        input->setHighlight(-1);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        output = new BinaryNumber;
        output->setBits ( fractionValue, 24 );
        table->setCellWidget(row,0,output);
        label = new QLabel(" Denormalized float has no implicit 1");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Fraction value"));
        doit = new QPushButton("to float");
        table->setCellWidget(row,1,doit);
        row++;
        toFloatState = BinaryValue;
        connect ( doit, SIGNAL(clicked()), this, SLOT(hexToFloat2()) );
        break;

    case Infinity:
        input->setHighlight(-1);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        if ( sign == 0 ) {
            label = new QLabel("positive infinity");
        } else {
            label = new QLabel("negative infinity");
        }
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(row,0,label);
        label = new QLabel(" Exponent == 255 && fraction == 0 ==> infinity");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("Infinity"));
        break;

    case NaN:
        input->setHighlight(-1);
        table->setRowCount(row+1);
        table->setCellWidget(row-1,1,new QLabel(""));
        if ( (value & 0x400000) == 0 ) {
            label = new QLabel("SNaN - signaling not-a-number");
        } else {
            label = new QLabel("QNaN - quiet not-a-number");
        }
        label->setAlignment(Qt::AlignCenter);
        table->setCellWidget(row,0,label);
        label = new QLabel(" Exponent == 255 && fraction != 0 ==> NaN");
        table->setCellWidget(row,2,label);
        table->setVerticalHeaderItem(row,new QTableWidgetItem("NaN"));

        break;



    }
}
