#include "unarybitpanel.h"
#include "validators.h"
#include "settings.h"

UnaryBitPanel::UnaryBitPanel(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10,10,10,10);


    operatorCombo = new QComboBox(this);

    QStringList ops;
    ops << "!  Boolean not" << "-  Negative" << "~  Bitwise not";
    operatorCombo->addItems(ops);

    QHBoxLayout *opLayout = new QHBoxLayout;
    opLayout->addWidget(new QLabel("Operator "));
    opLayout->addWidget(operatorCombo);
    opLayout->addStretch();

    layout->addLayout ( opLayout );

    table = new QTableWidget;

    //   input  do-it  format result comment
    table->setColumnCount(4);
    table->setRowCount(1);
    QStringList headers;
    headers << "Input" << "Operator" << "Result" << "Comment";
    table->setHorizontalHeaderLabels(headers);

    //number = new BinaryNumber;
    //number->setBits(32354,16);

    //layout->addWidget(number);

    input = new BinaryNumber;

    inputEdit = new IntegerEdit;

    fontSize = ebe["font_size"].toInt();
    table->setCellWidget(0,0,inputEdit);
    table->setCellWidget(0,2,input);
    table->setColumnWidth(0,8*fontSize);
    table->setColumnWidth(1,8*fontSize);
    table->setColumnWidth(2,16*fontSize+5);
    table->setColumnWidth(3,16*fontSize);
    
    layout->addWidget(table);

    setLayout(layout);

    connect ( operatorCombo, SIGNAL(activated(QString)),
              this, SLOT(selectOperator(QString)) );

    selectOperator("! Boolean not");

}

void UnaryBitPanel::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth  = width;
}

void UnaryBitPanel::selectOperator(QString o)
{
    input->clear();
    if ( o[0] == QChar('!') ) {
        op = o;
        doit = new QPushButton("!");
        table->setRowCount(1);
        table->setCellWidget(0,1,doit);
        table->setCellWidget(0,3,new QLabel(""));
        connect ( doit, SIGNAL(clicked()), this, SLOT(notStep1()) );
    } else if ( o[0] == QChar('-') ) {
        op = o;
        doit = new QPushButton("-");
        table->setRowCount(1);
        table->setCellWidget(0,1,doit);
        table->setCellWidget(0,3,new QLabel(""));
        connect ( doit, SIGNAL(clicked()), this, SLOT(negateStep1()) );
    } else if ( o[0] == QChar('~') ) {
        op = o;
        doit = new QPushButton("~");
        table->setRowCount(1);
        table->setCellWidget(0,1,doit);
        table->setCellWidget(0,3,new QLabel(""));
        connect ( doit, SIGNAL(clicked()), this, SLOT(bitwiseNotStep1()) );
    } else {
        qDebug() << "Unknown operator:" << op;
    }
}

void UnaryBitPanel::notStep1()
{
    value = inputEdit->value();
    input->setBits(value,16);
    table->setCellWidget(0,3,new QLabel("Value converted to binary"));
    table->setRowCount(2);
    table->setCellWidget(0,1,new QLabel(""));
    doit = new QPushButton("!");
    table->setCellWidget(1,1,doit);
    connect ( doit, SIGNAL(clicked()), this, SLOT(notStep2()) );
}

void UnaryBitPanel::notStep2()
{
    output1 = new BinaryNumber;
    output1->setBits(!value,8);
    table->setCellWidget(1,2,output1);
    table->setCellWidget(1,3,new QLabel("Result as a binary bool"));
    table->setCellWidget(1,1,new QLabel(""));
}

void UnaryBitPanel::bitwiseNotStep1()
{
    value = inputEdit->value();
    input->setBits(value,16);
    table->setCellWidget(0,3,new QLabel("Value converted to binary"));
    table->setRowCount(2);
    table->setCellWidget(0,1,new QLabel(""));
    doit = new QPushButton("~");
    table->setCellWidget(1,1,doit);
    connect ( doit, SIGNAL(clicked()), this, SLOT(bitwiseNotStep2()) );
}

void UnaryBitPanel::bitwiseNotStep2()
{
    output1 = new BinaryNumber;
    output1->setBits(~value,16);
    table->setCellWidget(1,2,output1);
    table->setCellWidget(1,3,new QLabel("All bits flipped"));
    table->setCellWidget(1,1,new QLabel(""));
}

void UnaryBitPanel::negateStep1()
{
    value = inputEdit->value();
    input->setBits(value,16);
    table->setCellWidget(0,3,new QLabel("Value converted to binary"));
    table->setRowCount(2);
    table->setCellWidget(0,1,new QLabel(""));
    doit = new QPushButton("-");
    table->setCellWidget(1,1,doit);
    connect ( doit, SIGNAL(clicked()), this, SLOT(negateStep2()) );
}

void UnaryBitPanel::negateStep2()
{
    output1 = new BinaryNumber;
    output1->setBits(~value,16);
    table->setRowCount(3);
    table->setCellWidget(1,2,output1);
    table->setCellWidget(1,3,new QLabel("All bits flipped"));
    table->setCellWidget(1,1,new QLabel(""));
    doit = new QPushButton("-");
    table->setCellWidget(2,1,doit);
    connect ( doit, SIGNAL(clicked()), this, SLOT(negateStep3()) );
}

void UnaryBitPanel::negateStep3()
{
    add1 = new BinaryNumber;
    add1->setText("+ 1",16);
    table->setCellWidget(2,2,add1);
    table->setCellWidget(2,3,new QLabel("Adding 1"));
    table->setRowCount(4);
    table->setCellWidget(2,1,new QLabel(""));
    doit = new QPushButton("-");
    table->setCellWidget(3,1,doit);
    connect ( doit, SIGNAL(clicked()), this, SLOT(negateStep4()) );
}

void UnaryBitPanel::negateStep4()
{
    output2 = new BinaryNumber;
    output2->setBits(-value,16);
    table->setCellWidget(3,2,output2);
    table->setCellWidget(3,3,new QLabel("Twos complement"));
    table->setCellWidget(3,1,new QLabel(""));
}
