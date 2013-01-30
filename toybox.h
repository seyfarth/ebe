#ifndef TOYBOX_H
#define TOYBOX_H

/*
    ToyBox class
*/

#include "types.h"
#include <QtGui>


class ToyBox;
class VariableTable;
class ExpressionTable;

/*
 * Design of the tables
 *
 * VariableName   Type   Value
 *      a         int      7
 * 
 * Expression     Doit   Value
 *      a/2       button   3
 */
class ToyVariable: public QObject
{
    Q_OBJECT

public:
    ToyVariable(VariableTable *t, int r);
    QLineEdit *name;
    QComboBox *type;
    QLineEdit *value;
    VariableTable *table;
    int row;
};

class VariableTable: public QTableWidget
{
public:
    VariableTable(ToyBox *p);
    void setFontHeightAndWidth ( int height, int width );
    ToyBox *box;
    int fontHeight;
    int fontWidth;
};

class ToyExpression: public QObject
{
    Q_OBJECT

public:
    ToyExpression(ExpressionTable *t, int r);

    QLineEdit *expr;
    QPushButton *button;
    QLineEdit *type;
    QComboBox *format;
    QLineEdit *value;
    ExpressionTable *table;
    int row;
    void setValue( QString t, QStringList v);
    void setValue();
    QStringList hex;
};

class ExpressionTable: public QTableWidget
{
    Q_OBJECT

public:
    ExpressionTable(ToyBox *p);
    void setFontHeightAndWidth ( int height, int width );
    ToyBox *box;
    int fontHeight;
    int fontWidth;
};

class ToyBox : public QFrame
{
    Q_OBJECT

public:
    ToyBox(QWidget *parent = 0);
    VariableTable *variableTable;
    ExpressionTable *expressionTable;

    void setFontHeightAndWidth ( int height, int width );
    QVector<ToyVariable *> variables;
    QVector<ToyExpression *> expressions;
    QString code;

public slots:
    void computeExpression();
    void formatResult(int);

private:

    QSize sizeHint() const;

    int fontWidth;          //< The width of a fixed width character
    int fontHeight;         //< The height of a fixed width character

};

class NumberValidator: public QValidator
{
public:
    NumberValidator();
    QValidator::State validate ( QString &s, int &pos ) const;
};

class IdValidator: public QValidator
{
public:
    IdValidator();
    QValidator::State validate ( QString &s, int &pos ) const;
};

union AllTypes
{
    double f8;
    float f4;
    unsigned long u8;
    long i8;
    unsigned int u4;
    int i4;
    unsigned short u2;
    short i2;
    unsigned char b[8];
    unsigned char u1;
    signed char i1;
    char c;
    bool b1;
};


#endif

