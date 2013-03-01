#ifndef TOYBOX_H
#define TOYBOX_H

/*
    ToyBox class
*/

#include <QtGui>
#include "types.h"
#include "validators.h"

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
    Q_OBJECT

public:
    VariableTable(ToyBox *p);
    void setFontHeightAndWidth ( int height, int width );
    ToyBox *box;
    int fontHeight;
    int fontWidth;

public slots:
    void switchLanguage(QString);
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
    QComboBox *languageCombo;
    VariableTable *variableTable;
    ExpressionTable *expressionTable;

    void setFontHeightAndWidth ( int height, int width );
    QVector<ToyVariable *> variables;
    QVector<ToyExpression *> expressions;
    QString code;
    QString language;

public slots:
    void computeExpression();
    void formatResult(int);
    void switchLanguage(QString);

private:

    QSize sizeHint() const;

    int fontWidth;          //< The width of a fixed width character
    int fontHeight;         //< The height of a fixed width character

};

#endif

