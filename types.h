#ifndef TYPES_H
#define TYPES_H
#include <QHash>
#include <QSet>
#include <QString>

typedef QHash<QString,QString> StringHash;
typedef QHash<QString,int> IntHash;
typedef QSet<QString> StringSet;
typedef QSet<int> IntSet;

union AllTypes
{
    double f8;
    float f4;
#ifdef Q_WS_WIN
    unsigned long long u8;
    long long i8;
#else
    unsigned long u8;
    long i8;
#endif
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

QString binary(AllTypes &a, int n);
QString binaryFloat(AllTypes &a);
QString binaryDouble(AllTypes &a);
QString fieldsFloat(AllTypes &a);
QString fieldsDouble(AllTypes &a);

#endif
