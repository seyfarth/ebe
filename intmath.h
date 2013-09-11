#ifndef INTMATH_H
#define INTMATH_H

#include <QtGui>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include "binarynumber.h"
#include "integeredit.h"

class IntMath : public QFrame
{
    Q_OBJECT

public:
    IntMath(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    int fontWidth;
    int fontHeight;
    QComboBox *mathCombo;
    BinaryNumber *input1;
    BinaryNumber *input2;
    BinaryNumber *carry;
    BinaryNumber *output;
    IntegerEdit *input1Edit;
    IntegerEdit *input2Edit;
    QPushButton *doit;

    QTableWidget *table;

    int value1;
    int value2;
    int computedValue;
    int bit;
    int row;
    int fontSize;

    QString op;

public slots:
    void selectOperator(QString);
    void add1();
    void add2();
    void multiply1();
    void multiply2();

private:

signals:
};

#endif
