#include "validators.h"
#include "errorwindow.h"

NumberValidator::NumberValidator()
: QValidator()
{
}

QValidator::State NumberValidator::validate(QString &s, int & /* pos */ ) const
{
    QString t = s.trimmed();
    if ( t == "" ) return QValidator::Intermediate;
    if ( t == "-" ) return QValidator::Intermediate;
    if ( t == "'" ) return QValidator::Intermediate;
    if ( t == "+" ) return QValidator::Intermediate;
    if ( t == "0x" ) return QValidator::Intermediate;
    if ( t == "0b" ) return QValidator::Intermediate;
    QRegExp rx1(QString("[\\+\\-]?[0-9]+"));
    if ( rx1.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx2(QString("0x[0-9a-fA-F]+"));
    if ( rx2.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx3(QString("0b[01]+"));
    if ( rx3.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx4(QString("[\\+\\-]?[0-9]+\\.[0-9]+"));
    if ( rx4.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx5(QString("[\\+\\-]?[0-9]*\\.[0-9]*"));
    if ( rx5.exactMatch(t) ) return QValidator::Intermediate;
    QRegExp rx6(QString("'."));
    if ( rx6.exactMatch(t) ) return QValidator::Intermediate;
    QRegExp rx7(QString("'.'"));
    if ( rx7.exactMatch(t) ) return QValidator::Acceptable;
    if ( t == "t" || t == "tr" || t == "tru" ) return QValidator::Intermediate;
    if ( t == "true" ) return QValidator::Acceptable;
    if ( t == "f" || t == "fa" || t == "fal" ||
         t == "fals" ) return QValidator::Intermediate;
    if ( t == "false" ) return QValidator::Acceptable;
    return QValidator::Invalid;
}

IntegerValidator::IntegerValidator()
: QValidator()
{
}

QValidator::State IntegerValidator::validate(QString &s, int & /* pos */ ) const
{
    QString t = s.trimmed();
    if ( t == "" ) return QValidator::Intermediate;
    if ( t == "-" ) return QValidator::Intermediate;
    if ( t == "+" ) return QValidator::Intermediate;
    if ( t == "0x" ) return QValidator::Intermediate;
    if ( t == "0b" ) return QValidator::Intermediate;
    QRegExp rx1(QString("[\\+\\-]?[0-9]+"));
    if ( rx1.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx2(QString("0x[0-9a-fA-F]+"));
    if ( rx2.exactMatch(t) ) return QValidator::Acceptable;
    QRegExp rx3(QString("0b[01]+"));
    if ( rx3.exactMatch(t) ) return QValidator::Acceptable;
    if ( t == "t" || t == "tr" || t == "tru" ) return QValidator::Intermediate;
    if ( t == "true" ) return QValidator::Acceptable;
    if ( t == "f" || t == "fa" || t == "fal" ||
         t == "fals" ) return QValidator::Intermediate;
    if ( t == "false" ) return QValidator::Acceptable;
    return QValidator::Invalid;
}

FloatValidator::FloatValidator()
: QValidator()
{
}

QValidator::State FloatValidator::validate(QString &s, int & /* pos */ ) const
{
    QString t = s.trimmed();
    if ( t == "" ) return QValidator::Intermediate;
    if ( t == "-" ) return QValidator::Intermediate;
    if ( t == "+" ) return QValidator::Intermediate;
    QRegExp rx1(QString("0x[0-9a-fA-F]{8}"));
    QRegExp rx2(QString("0x[0-9a-fA-F]{0,7}"));
    QRegExp rx3(QString("[-+]?[0-9]+(\\.[0-9]*)?"));
    QRegExp rx4(QString("[-+]?[0-9]*(\\.[0-9]*)"));
    if ( rx1.exactMatch(t) ) return QValidator::Acceptable;
    if ( rx2.exactMatch(t) ) return QValidator::Intermediate;
    if ( rx3.exactMatch(t) ) return QValidator::Acceptable;
    if ( rx4.exactMatch(t) ) return QValidator::Acceptable;
    return QValidator::Invalid;
}

IdValidator::IdValidator()
: QValidator()
{
}

QValidator::State IdValidator::validate(QString &s, int & /* pos */ ) const
{
    QString t = s.trimmed();
    if ( t == "" ) return QValidator::Intermediate;
    QRegExp rx1(QString("[a-zA-Z][a-zA-Z0-9_]*"));
    if ( rx1.exactMatch(t) ) return QValidator::Acceptable;
    return QValidator::Invalid;
}
