#ifndef TYPES_H
#define TYPES_H
#include <QHash>
#include <QSet>
#include <QString>

typedef QHash<QString, QString> StringHash;
typedef QHash<QString, int> IntHash;
typedef QSet<QString> StringSet;
typedef QSet<int> IntSet;

#ifdef Q_WS_WIN
    typedef unsigned long long uLong;
    typedef long long sLong;
#else
    typedef unsigned long uLong;
    typedef long sLong;
#endif
union AllTypes
{
    double f8;
    float f4;
    uLong u8;
    sLong i8;
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

struct Range
{
    int first;
    int last;
};

QString binary(AllTypes &a, int n);
QString hexFloat(AllTypes &a);
QString hexDouble(AllTypes &a);
QString binaryFloat(AllTypes &a);
QString binaryDouble(AllTypes &a);
QString fieldsFloat(AllTypes &a);
QString fieldsDouble(AllTypes &a);

class FileLine
{
public:
    QString file;
    int line;
    FileLine(QString f = "", int l = 0);
};

bool operator<(const FileLine &a, const FileLine &b);

bool operator==(const FileLine &a, const FileLine &b);

uint qHash(const FileLine &f);

class FileLabel
{
public:
    QString file;
    QString label;
    FileLabel(QString f = "", QString l = 0);
};

bool operator==(const FileLabel &a, const FileLabel &b);
bool operator<(const FileLabel &a, const FileLabel &b);

uint qHash(const FileLabel &f);

#endif
