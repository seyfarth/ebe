#ifndef FLOATCONVERT_H
#define FLOATCONVERT_H

#include <QtGui>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include "binarynumber.h"
#include "floatedit.h"

enum ToFloatState
{
    Zero,
    NegativeZero,
    Bits,
    SignBit,
    ExponentField,
    ExponentValue,
    FractionField,
    FractionValue,
    BinaryValue,
    BitValue,
    Result,
    Denormalized,
    Infinity,
    NaN
};

class FloatConvert: public QFrame
{
    Q_OBJECT

public:
    FloatConvert(QWidget *parent = 0);
    void setFontHeightAndWidth(int height, int width);
    int fontWidth;
    int fontHeight;
    QComboBox *convertCombo;
    BinaryNumber *input;
    BinaryNumber *output;
    FloatEdit *inputEdit;
    QPushButton *doit;

    QTableWidget *table;

    int value;
    int exponentField;
    int exponentValue;
    int computedValue;
    int fractionField;
    int fractionValue;
    int wholePart;
    int fractionPart;
    int bit;
    int row;
    int nibble;
    int fontSize;
    int sign;
    ToFloatState toFloatState;
    double bitValue;
    double result;

    QString op;

public slots:
    void selectOperator(QString);
    void hexToFloat1();
    void hexToFloat2();

private:

    signals:
};

#endif
