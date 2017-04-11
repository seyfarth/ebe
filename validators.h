#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <QValidator>
#include <QString>
#include <QRegExp>

class NumberValidator: public QValidator
{
public:
    NumberValidator();
    QValidator::State validate(QString &s, int &pos) const;
};

class IntegerValidator: public QValidator
{
public:
    IntegerValidator();
    QValidator::State validate(QString &s, int &pos) const;
};

class FloatValidator: public QValidator
{
public:
    FloatValidator();
    QValidator::State validate(QString &s, int &pos) const;
};

class IdValidator: public QValidator
{
public:
    IdValidator();
    QValidator::State validate(QString &s, int &pos) const;
};

#endif

