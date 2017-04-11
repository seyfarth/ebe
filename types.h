#ifndef TYPES_H
#define TYPES_H
#include <QHash>
#include <QSet>
#include <QString>
#include <QStringList>

typedef QHash<QString, QString> StringHash;
typedef QHash<QString, int> IntHash;
typedef QSet<QString> StringSet;
typedef QSet<int> IntSet;

#ifdef Q_OS_WIN32
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

union AllTypes16
{
    double         f8[2];
    float          f4[4];
    sLong          i8[2];
    int            i4[4];
    short          i2[8];
    char           c1[16];
    signed char    i1[16];
    uLong          u8[2];
    unsigned int   u4[4];
    unsigned short u2[8];
    unsigned char  u1[16];
};

class AllTypesArray
{
public:
    AllTypesArray(int size=8);
    AllTypesArray(AllTypesArray &x);
    int              size;  // in bytes
    uLong           *data;
    void             resize(int size);
    double         & f8(int i);
    float          & f4(int i);
    sLong          & i8(int i);
    int            & i4(int i);
    short          & i2(int i);
    char           & c1(int i);
    signed char    & i1(int i);
    uLong          & u8(int i);
    unsigned int   & u4(int i);
    unsigned short & u2(int i);
    unsigned char  & u1(int i);
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
QString hexFloat(float f);
QString hexDouble(double d);
QString binaryFloat(float f);
QString binaryDouble(double d);
QString fieldsFloat(float f);
QString fieldsDouble(double d);

QString toString(AllTypesArray *a, QStringList &v, int k, int j);
QString toChar(AllTypesArray *a, QStringList &v, int k, int j);
QString toHex1(AllTypesArray *a, QStringList &v, int k, int j);
QString toHex2(AllTypesArray *a, QStringList &v, int k, int j);
QString toHex4(AllTypesArray *a, QStringList &v, int k, int j);
QString toHex8(AllTypesArray *a, QStringList &v, int k, int j);
QString toDec1(AllTypesArray *a, QStringList &v, int k, int j);
QString toDec2(AllTypesArray *a, QStringList &v, int k, int j);
QString toDec4(AllTypesArray *a, QStringList &v, int k, int j);
QString toDec8(AllTypesArray *a, QStringList &v, int k, int j);
QString toUDec1(AllTypesArray *a, QStringList &v, int k, int j);
QString toUDec2(AllTypesArray *a, QStringList &v, int k, int j);
QString toUDec4(AllTypesArray *a, QStringList &v, int k, int j);
QString toUDec8(AllTypesArray *a, QStringList &v, int k, int j);
QString toFloat(AllTypesArray *a, QStringList &v, int k, int j);
QString toDouble(AllTypesArray *a, QStringList &v, int k, int j);
QString toBinaryFP4(AllTypesArray *a, QStringList &v, int k, int j);
QString toBinaryFP8(AllTypesArray *a, QStringList &v, int k, int j);
QString toFields4(AllTypesArray *a, QStringList &v, int k, int j);
QString toFields8(AllTypesArray *a, QStringList &v, int k, int j);
QString toBin1(AllTypesArray *a, QStringList &v, int k, int j);
QString toBin2(AllTypesArray *a, QStringList &v, int k, int j);
QString toBin4(AllTypesArray *a, QStringList &v, int k, int j);
QString toBin8(AllTypesArray *a, QStringList &v, int k, int j);

typedef QString (*FormatFunction)(AllTypesArray*,QStringList&,int,int);
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
