#ifndef INTEGEREDIT_H
#define INTEGEREDIT_H

#include <QLineEdit>

#include "validators.h"

class IntegerEdit : public QLineEdit
{
public:
    IntegerEdit();
    IntegerValidator *val;
    int value();
    void setValue(int,int=10);
    void setValue(bool);
    bool isValid();
};

#endif
