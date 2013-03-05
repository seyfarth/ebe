#include "intconvert.h"
#include "settings.h"

IntConvert::IntConvert(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10,10,10,10);


    convertCombo = new QComboBox(this);

    QStringList ops;
    ops << "Decimal to Binary" << "Decimal to Hex"
        << "Binary to Decimal"  << "Hex to Decimal"
        << "Binary to Hex" << "Hex to Binary";

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

    selectOperator("Decimal to Binary");

}

void IntConvert::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth  = width;
}

void IntConvert::selectOperator(QString o)
{
    QStringList headers;

    if ( o == "Decimal to Binary" ) {
        op = o;
        table->clear();
        table->setColumnCount(7);
        table->setRowCount(1);
        inputEdit = new IntegerEdit;
        table->setCellWidget(0,0,inputEdit);
        headers << "Input" << "Conversion" << " n " << "n / 2" << "n % 2"
                << "Result" << "Comment";
        table->setHorizontalHeaderLabels(headers);
        doit = new QPushButton("to binary");
        table->setCellWidget(0,1,doit);
        connect ( doit, SIGNAL(clicked()), this, SLOT(decimalToBinary1()) );
    } else {
        qDebug() << "Unknown operator:" << op;
    }
}

void IntConvert::decimalToBinary1()
{
    QLabel *label;
    value = inputEdit->value();
    computedValue = 0;
    bit = 0;
    table->setCellWidget(0,6,new QLabel("Value copied into column 3"));
    label = new QLabel(QString("%1").arg(value));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(0,2,label);
    connect ( doit, SIGNAL(clicked()), this, SLOT(decimalToBinary2()) );
}

void IntConvert::decimalToBinary2()
{
    int n = value % 2;
    QLabel *label;

    output = new BinaryNumber;
    computedValue = (n << bit) + computedValue;
    output->setBits(computedValue,16,bit+1);
    
    table->setCellWidget(bit,1,new QLabel(""));
    doit->setText("");
    doit->setDisabled(true);
    label = new QLabel(QString("%1").arg(value/2));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(bit,3,label);
    label = new QLabel(QString("%1").arg(value%2));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(bit,4,label);
    table->setCellWidget(bit,5,output);
    table->setCellWidget(bit,6,new QLabel("divide by 2"));
    
    value = value >> 1;
    if ( value == 0 ) return;
    bit++;
    table->setRowCount(bit+1);
    doit = new QPushButton("to binary");
    table->setCellWidget(bit,1,doit);
    label = new QLabel(QString("%1").arg(value));
    label->setAlignment(Qt::AlignCenter);
    table->setCellWidget(bit,0,new QLabel(""));
    table->setCellWidget(bit,2,label);
    table->setCellWidget(bit,3,new QLabel(""));
    table->setCellWidget(bit,4,new QLabel(""));
    table->setCellWidget(bit,5,new QLabel(""));
    connect ( doit, SIGNAL(clicked()), this, SLOT(decimalToBinary2()) );
    table->setCellWidget(bit,6,new QLabel("moved n/2 to n in new row"));
}
