#ifndef INTCONVERT_H
#define INTCONVERT_H

#include <QtGui>
#include "binarynumber.h"
#include "integeredit.h"

class IntConvert : public QFrame
{
    Q_OBJECT

public:
    IntConvert(QWidget *parent=0);
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
    int fontSize;

    QString op;

public slots:
    void selectOperator(QString);
    void decimalToBinary1();
    void decimalToBinary2();

private:

signals:
};

#endif
