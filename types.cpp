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

QString hexFloat(float f)
{
    AllTypes a;
    a.f4 = f;
    return hexFloat(a);
}

QString hexDouble(AllTypes &a)
{
    return QString::number(a.u8, 16);
}

QString hexDouble(double d)
{
    AllTypes a;
    a.f8 = d;
    return hexDouble(a);
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

QString binaryFloat(float f)
{
    AllTypes a;
    a.f4 = f;
    return binaryFloat(a);
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

QString binaryDouble(double d)
{
    AllTypes a;
    a.f8 = d;
    return binaryDouble(a);
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

QString fieldsFloat(float f)
{
    AllTypes a;
    a.f4 = f;
    return fieldsFloat(a);
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

QString fieldsDouble(double d)
{
    AllTypes a;
    a.f8 = d;
    return fieldsDouble(a);
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

void AllTypesArray::resize(int n)
{
    int oldSize = size;
    size = n;
    if ( size < 8 ) size = 8;
    if ( size != oldSize ) {
        int n = (size+7) / 8;
        uLong *newData = new uLong[n];
        n = (oldSize+7) / 8;
        for ( int i=0; i < n; i++ ) newData[i] = data[i];
        delete [] data;
        data = newData;
        newData = 0;
    }
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

QString toString(AllTypesArray *, QStringList &v, int k, int)
{
    if ( k >= v.length() ) return "";
    return v[k];
}

QString toChar(AllTypesArray *a, QStringList&, int k, int)
{
    int n=a->c1(k);
    if ( isprint(n) ) return QString(QChar(n));
    else if ( n == '\t' ) return "\\t";
    else if ( n == '\r' ) return "\\r";
    else if ( n == '\n' ) return "\\n";
    else return QString("%1").arg(a->u1(k),2,16,QChar('0'));
}

QString toHex1(AllTypesArray *a, QStringList&, int k, int j)
{
    if ( j == 0 ) return QString("0x%1").arg(a->u1(k),2,16,QChar('0'));
    else return QString("%1").arg(a->u1(k),2,16,QChar('0'));
}

QString toHex2(AllTypesArray *a, QStringList&, int k, int j)
{
    if ( j == 0 ) return QString("0x%1").arg(a->u2(k),2,16,QChar('0'));
    else return QString("%1").arg(a->u2(k),2,16,QChar('0'));
}

QString toHex4(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u4(k),2,16,QChar('0'));
}

QString toHex8(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u8(k),2,16,QChar('0'));
}

QString toDec1(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->i1(k));
}

QString toDec2(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->i2(k));
}

QString toDec4(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->i4(k));
}

QString toDec8(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->i8(k));
}

QString toUDec1(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u1(k));
}

QString toUDec2(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u2(k));
}

QString toUDec4(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u4(k));
}

QString toUDec8(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u8(k));
}

QString toFloat(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->f4(k),0,'g',7);
}

QString toDouble(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->f8(k),0,'g',9);
}

QString toBinaryFP4(AllTypesArray *a, QStringList&, int k, int)
{
    return binaryFloat(a->f4(k));
}

QString toBinaryFP8(AllTypesArray *a, QStringList&, int k, int)
{
    return binaryFloat(a->f8(k));
}

QString toFields4(AllTypesArray *a, QStringList&, int k, int)
{
    return fieldsFloat(a->f4(k));
}

QString toFields8(AllTypesArray *a, QStringList&, int k, int)
{
    return fieldsDouble(a->f8(k));
}

QString toBin1(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u1(k),8,2,QChar('0'));
}

QString toBin2(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u1(k),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+1),8,2,QChar('0'));
}

QString toBin4(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u1(k),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+1),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+2),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+3),8,2,QChar('0'));
}

QString toBin8(AllTypesArray *a, QStringList&, int k, int)
{
    return QString("%1").arg(a->u1(k),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+1),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+2),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+3),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+4),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+5),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+6),8,2,QChar('0')) + " " +
           QString("%1").arg(a->u1(k+7),8,2,QChar('0'));
}
