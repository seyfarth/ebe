#include "integeredit.h"

IntegerEdit::IntegerEdit()
: QLineEdit()
{
}

int IntegerEdit::value()
{
    bool ok;
    QString t = text();
    if ( t.length() >= 2 && t.left(2) == "0x" ) {
        return t.mid(2).toInt(&ok,16);
    } else if ( t.length() >= 2 && t.left(2) == "0b" ) {
        return t.mid(2).toInt(&ok,2);
    } else {
        return t.toInt();
    }
}

void IntegerEdit::setValue(int n,int base)
{
    QString number;
    unsigned int x;
    if ( base == 10 ) {
        number.sprintf("%d",n);
    } else if ( base == 16 ) {
        number.sprintf("0x%x",n);
    } else {
        if ( number == 0 ) {
            number = "0xb0";
        } else {
            x = (unsigned int)n;
            while ( x ) {
                number = (x&1)?"1":"0" + number;
                x >>= 1;
            }
            number = "0xb" + number;
        }
    }
}

void IntegerEdit::setValue(bool b)
{
    setText(b?"true":"false");
}
