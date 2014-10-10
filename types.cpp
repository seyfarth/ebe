#include "types.h"

FileLine::FileLine(QString f, int l)
    : file(f), line(l)
{
}

bool operator==(const FileLine &a, const FileLine &b)
{
    return a.file == b.file && a.line == b.line;
}

uint qHash(const FileLine &f)
{
    return qHash(f.file) ^ qHash(f.line);
}

bool operator<(const FileLine &a, const FileLine &b)
{
    if (a.file == b.file) return a.line < b.line;
    return a.file < b.file;
}

FileLabel::FileLabel(QString f, QString l)
    : file(f), label(l)
{
}

bool operator==(const FileLabel &a, const FileLabel &b)
{
    return a.file == b.file && a.label == b.label;
}

uint qHash(const FileLabel &f)
{
    return qHash(f.file) ^ qHash(f.label);
}

bool operator<(const FileLabel &a, const FileLabel &b)
{
    if (a.file == b.file) return a.label < b.label;
    return a.file < b.file;
}

QString binary(AllTypes &a, int n)
{
    QString s = "";
    int mask;
    for (int i = 0; i < n; i++) {
        mask = 0x80;
        for (int b = 0; b < 8; b++) {
            s += a.b[n - i - 1] & mask ? "1" : "0";
            mask >>= 1;
        }
        if (i < n - 1) s += " ";
    }
    return s;
}

QString hexFloat(AllTypes &a)
{
    return QString::number(a.u4, 16);
}

QString hexDouble(AllTypes &a)
{
    return QString::number(a.u8, 16);
}

QString binaryFloat(AllTypes &a)
{
    QString s = "";
    int sign, exp;

    sign = a.u4 >> 31;
    exp = ((a.u4 >> 23) & 0xff) - 127;
    s = sign == 1 ? "-" : " ";
    s += "1.";
    for (int i = 22; i >= 0; i--) {
        s += (a.u4 & (1 << i)) != 0 ? "1" : "0";
    }
    s += QString(" * 2**%1").arg(exp);
    return s;
}

QString binaryDouble(AllTypes &a)
{
    QString s = "";
    int sign, exp;

    sign = a.u8 >> 63;
    //qDebug() << "sign" << sign;
    exp = ((a.u8 >> 52) & 0x7ff) - 1023;
    //qDebug() << "exp" << exp;
    s = sign == 1 ? "-" : " ";
    s += "1.";
    for (int i = 51; i >= 0; i--) {
        //printf("%lx %lx \n",a.u8,1UL<<i);
        s += ((a.u8 >> i) & 1) == 1 ? "1" : "0";
    }
    s += QString(" * 2**%1").arg(exp);
    return s;
}

QString fieldsFloat(AllTypes &a)
{
    QString s = "";
    int b;
    s = (a.u4 >> 31) == 1 ? "1:" : "0:";
    for (b = 30; b >= 23; b--) {
        s += (a.u4 & (1 << b)) != 0 ? "1" : "0";
    }
    s += ":";
    for (b = 22; b >= 0; b--) {
        s += (a.u4 & (1 << b)) != 0 ? "1" : "0";
    }
    return s;
}

QString fieldsDouble(AllTypes &a)
{
    QString s = "";
    int b;
    s = (a.u8 >> 63) == 1 ? "1:" : "0:";
    for (b = 62; b >= 52; b--) {
        s += ((a.u8 >> b) & 1) == 1 ? "1" : "0";
    }
    s += ":";
    for (b = 51; b >= 0; b--) {
        s += ((a.u8 >> b) & 1) == 1 ? "1" : "0";
    }
    return s;
}

