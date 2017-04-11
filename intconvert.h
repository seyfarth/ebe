#ifndef INTCONVERT_H
#define INTCONVERT_H

#include <QtGui>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>

#include "binarynumber.h"
#include "integeredit.h"

class IntConvert: public QFrame
{
    Q_OBJECT

public:
    IntConvert(QWidget *parent = 0);
    void setFontHeightAndWidth(int height, int width);
    int fontWidth;
    int fontHeight;
    QComboBox *convertCombo;
    BinaryNumber *input;
    BinaryNumber *output;
    IntegerEdit *inputEdit;
    QPushButton *doit;

    QTableWidget *table;

    int value;
    int computedValue;
    int bit;
    int row;
    int nibble;
    int fontSize;

    QString op;

public slots:
    void selectOperator(QString);
    void decimalToBinary1();
    void decimalToBinary2();
    void decimalToHex1();
    void decimalToHex2();
    void binaryToDecimal1();
    void binaryToDecimal2();
    void hexToDecimal1();
    void hexToDecimal2();
    void binaryToHex1();
    void binaryToHex2();
    void hexToBinary1();
    void hexToBinary2();

private:

    signals:
};

#endif
