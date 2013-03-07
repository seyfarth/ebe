#include "intmath.h"
#include "settings.h"

IntMath::IntMath(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10,10,10,10);

    mathCombo = new QComboBox(this);

    QStringList ops;
    ops << "Addition" << "Multiplication";

    mathCombo->addItems(ops);

    QHBoxLayout *opLayout = new QHBoxLayout;
    opLayout->addWidget(new QLabel("Operation "));
    opLayout->addWidget(mathCombo);
    opLayout->addStretch();

    layout->addLayout ( opLayout );

    table = new QTableWidget;

    //   input  do-it  format result comment
    layout->addWidget(table);

    setLayout(layout);

    connect ( mathCombo, SIGNAL(activated(QString)),
              this, SLOT(selectOperator(QString)) );


    selectOperator("Addition");
}

void IntMath::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth  = width;
}

void IntMath::selectOperator(QString o)
{
    QStringList headers;

    fontSize = ebe["font_size"].toInt();
    if ( o == "Addition" ) {
        op = o;
        input1 = new BinaryNumber;
        input2 = new BinaryNumber;
        output = new BinaryNumber;
        carry  = new BinaryNumber;
        table->clear();
        table->setColumnCount(4);
        table->setRowCount(4);
        //for ( int r = 0; r < 4; r++ ) {
            //for ( int c = 0; c < 4; c++ ) {
                //table->setCellWidget(r,c,new QLabel(""));
            //}
        //}
        input1Edit = new IntegerEdit;
        table->setCellWidget(1,0,input1Edit);
        input2Edit = new IntegerEdit;
        table->setCellWidget(2,0,input2Edit);
        headers << "Input" << "Operator" << "Bits" << "Comment";
        table->setHorizontalHeaderLabels(headers);
        doit = new QPushButton("+");
        table->setCellWidget(3,1,doit);
        table->setColumnWidth(0,8*fontSize);
        table->setColumnWidth(1,6*fontSize);
        table->setColumnWidth(2,17*fontSize+5);
        table->setColumnWidth(3,20*fontSize);
        connect ( doit, SIGNAL(clicked()), this, SLOT(add1()) );
    } else {
        qDebug() << "Unknown operator:" << o;
    }
}

void IntMath::add1()
{
    if ( ! input1Edit->isValid() ) {
        QMessageBox::warning(this,tr("Warning"),
            tr("The first input value is not complete."),
            QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if ( ! input2Edit->isValid() ) {
        QMessageBox::warning(this,tr("Warning"),
            tr("The second input value is not complete."),
            QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    value1 = input1Edit->value();
    value1 = value1 & 0xffff;
    value2 = input2Edit->value();
    value2 = value2 & 0xffff;
    bit = 0;
    table->setCellWidget(0,3,new QLabel(" Carries"));
    table->setCellWidget(1,3,new QLabel(" Value & 0xffff copied into column 3"));
    table->setCellWidget(2,3,new QLabel(" Value & 0xffff copied into column 3"));
    table->setCellWidget(3,3,new QLabel(" Sum"));
    carry->clear();
    carry->setBits ( value2, 17, 0 );
    table->setCellWidget(0,2,carry);
    input1->clear();
    input1->setBits ( value1, 17, 16 );
    table->setCellWidget(1,2,input1);
    input2->clear();
    input2->setBits ( value2, 17, 16 );
    table->setCellWidget(2,2,input2);
    output->clear();
    output->setBits ( value1+value2, 17, 0 );
    table->setCellWidget(3,2,output);
    doit->disconnect(SIGNAL(clicked()));
    connect ( doit, SIGNAL(clicked()), this, SLOT(add2()) );
    table->resizeColumnsToContents();
}

void IntMath::add2()
{
    int mask = 0;
    for ( int i = 0; i <= bit; i++ ) mask |= 1 << i;

    int n1 = value1 & mask;
    int n2 = value2 & mask;
    int sum = n1 + n2;
    int c = sum & (1 << (bit+1)) ? 1 : 0;
    if ( c ) carry->setBit(c,bit+1);
    output->setBits(sum,17,bit+1);
    table->update();

    bit++;
    if ( bit == 16 ) {
        table->setCellWidget(3,1,new QLabel(""));
        return;
    }
    table->resizeColumnsToContents();
}
