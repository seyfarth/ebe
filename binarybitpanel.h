#ifndef BINARYBITPANEL_H
#define BINARYBITPANEL_H

#include <QtGui>
#include <QTableWidget>
#include <QComboBox>
#include <QPushButton>
#include "binarynumber.h"
#include "integeredit.h"

class BinaryBitPanel : public QFrame
{
    Q_OBJECT

public:
    BinaryBitPanel(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    int fontWidth;
    int fontHeight;
    QComboBox *operatorCombo;
    BinaryNumber *input1;
    BinaryNumber *input2;
    BinaryNumber *output;
    IntegerEdit *input1Edit;
    IntegerEdit *input2Edit;
    QPushButton *doit;

    QTableWidget *table;

    int value1;
    int value2;
    int count;
    int computedValue;
    int fontSize;

    QString op;

public slots:
    void selectOperator(QString);
    void orStep1();
    void orStep2();
    void bitwiseOrStep1();
    void bitwiseOrStep2();
    void andStep1();
    void andStep2();
    void bitwiseAndStep1();
    void bitwiseAndStep2();
    void xorStep1();
    void xorStep2();
    void shiftLeftStep1();
    void shiftLeftStep2();
    void signedShiftRightStep1();
    void signedShiftRightStep2();
    void shiftRightStep1();
    void shiftRightStep2();
    void rotateLeftStep1();
    void rotateLeftStep2();
    void rotateRightStep1();
    void rotateRightStep2();

private:

signals:
};

#endif
