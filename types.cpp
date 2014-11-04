#include "types.h"
#include <QDebug>

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


AllTypesArray::AllTypesArray(int size_)
{
    size = size_;
    if ( size < 8 ) size = 8;
    int n = (size+7) / 8;
    data = new uLong[n];
    for ( int i = 0; i < n; i++ ) data[i] = 0;
}

AllTypesArray::AllTypesArray(AllTypesArray &x)
{
    size = x.size;
    int n = (size+7) / 8;
    data = new uLong[n];
    for ( int i = 0; i < n; i++ ) data[i] = x.data[i];
}

double & AllTypesArray::f8(int i)
{
    int n = (size+7) / 8;
    if ( i < 0 || i >= n ) {
        qDebug() << QString("f8(%1) is out of range: 0:%2").arg(i).arg(n-1);
        i = 0;
    }
    return ((double *)data)[i];
}

float & AllTypesArray::f4(int i)
{
    int n = (size+3) / 4;
    if ( i < 0 || i >= n ) {
        qDebug() << QString("f4(%1) is out of range: 0:%2").arg(i).arg(n-1);
        i = 0;
    }
    return ((float *)data)[i];
}

sLong & AllTypesArray::i8(int i)
{
    int n = (size+7) / 8;
    if ( i < 0 || i >= n ) {
        qDebug() << QString("i8(%1) is out of range: 0:%2").arg(i).arg(n-1);
        i = 0;
    }
    return ((sLong *)data)[i];
}

int & AllTypesArray::i4(int i)
{
    int n = (size+3) / 4;
    if ( i < 0 || i >= n ) {
        qDebug() << QString("i4(%1) is out of range: 0:%2").arg(i).arg(n-1);
        i = 0;
    }
    return ((int *)data)[i];
}

short & AllTypesArray::i2(int i)
{
    int n = (size+1) / 2;
    if ( i < 0 || i >= n ) {
        qDebug() << QString("i2(%1) is out of range: 0:%2").arg(i).arg(n-1);
        i = 0;
    }
    return ((short *)data)[i];
}

char & AllTypesArray::c1(int i)
{
    if ( i < 0 || i >= size ) {
        qDebug() << QString("c1(%1) is out of range: 0:%2").arg(i).arg(size-1);
        i = 0;
    }
    return ((char *)data)[i];
}

signed char & AllTypesArray::i1(int i)
{
    if ( i < 0 || i >= size ) {
        qDebug() << QString("i1(%1) is out of range: 0:%2").arg(i).arg(size-1);
        i = 0;
    }
    return ((signed char *)data)[i];
}

uLong & AllTypesArray::u8(int i)
{
    int n = (size+7) / 8;
    if ( i < 0 || i >= n ) {
        qDebug() << QString("u8(%1) is out of range: 0:%2").arg(i).arg(n-1);
        i = 0;
    }
    return ((uLong *)data)[i];
}

unsigned int & AllTypesArray::u4(int i)
{
    int n = (size+3) / 4;
    if ( i < 0 || i >= n ) {
        qDebug() << QString("u4(%1) is out of range: 0:%2").arg(i).arg(n-1);
        i = 0;
    }
    return ((unsigned int *)data)[i];
}

unsigned short & AllTypesArray::u2(int i)
{
    int n = (size+1) / 2;
    if ( i < 0 || i >= n ) {
        qDebug() << QString("u2(%1) is out of range: 0:%2").arg(i).arg(n-1);
        i = 0;
    }
    return ((unsigned short *)data)[i];
}

unsigned char & AllTypesArray::u1(int i)
{
    if ( i < 0 || i >= size ) {
        qDebug() << QString("u1(%1) is out of range: 0:%2").arg(i).arg(size-1);
        i = 0;
    }
    return ((unsigned char *)data)[i];
}
