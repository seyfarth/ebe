#include "floatedit.h"
#include "validators.h"
#include "types.h"
#include <QDebug>

FloatEdit::FloatEdit()
: QLineEdit()
{
    val = new FloatValidator;
    setValidator(val);
    setToolTip(tr("Enter an float value or 0xDEADBEEF (8 hex nibbles)"));
}

bool FloatEdit::isValid()
{
    QString t=text();
    int pos=0;
    return val->validate(t,pos) == QValidator::Acceptable;
}

int FloatEdit::value()
{
    bool ok;
    QString t = text();
    AllTypes x;
    x.i8 = 0;
    if ( t.left(2) == "0x" || t.left(2) == "0X" ) {
        x.i8 = t.toULongLong(&ok,16);
    } else {
        x.f4 = t.toFloat();
    }
    return x.i4;
}

void FloatEdit::setValue(int n)
{
    QString number;
    number.sprintf("0x%08x",n);
    setText(number);
}
