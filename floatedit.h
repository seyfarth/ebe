#ifndef FLOATEDIT_H
#define FLOATEDIT_H

#include <QLineEdit>

#include "validators.h"

class FloatEdit : public QLineEdit
{
public:
    FloatEdit();
    FloatValidator *val;
    int value();
    void setValue(int);
    bool isValid();
};

#endif
