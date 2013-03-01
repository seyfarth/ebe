#ifndef UNARYBITPANEL_H
#define UNARYBITPANEL_H

#include <QtGui>
#include "binarynumber.h"
#include "integeredit.h"

class UnaryBitPanel : public QFrame
{
    Q_OBJECT

public:
    UnaryBitPanel(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    int fontWidth;
    int fontHeight;
    QComboBox *operatorCombo;
    BinaryNumber *input;
    BinaryNumber *output1;
    BinaryNumber *output2;
    BinaryNumber *add1;
    IntegerEdit *inputEdit;
    QPushButton *doit;

    QTableWidget *table;

    int value;
    int computedValue;
    int fontSize;

    QString op;

public slots:
    void selectOperator(QString);
    void notStep1();
    void notStep2();
    void bitwiseNotStep1();
    void bitwiseNotStep2();
    void negateStep1();
    void negateStep2();
    void negateStep3();
    void negateStep4();

private:

signals:
};

#endif
