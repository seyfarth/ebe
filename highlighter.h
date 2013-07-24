#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QSet>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(QTextDocument *parent = 0);

protected:
    QSet<QString> keywords;
    QString operators;

    QTextCharFormat idFormat;
    QTextCharFormat keywordFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat illegalFormat;
    QTextCharFormat preprocessorFormat;
};

class AsmHighlighter : public Highlighter
{
public:
    AsmHighlighter(QTextDocument *parent = 0);
protected:
    void highlightBlock(const QString &text);

};

class CppHighlighter : public Highlighter
{
public:
    CppHighlighter(QTextDocument *parent = 0);
protected:
    void highlightBlock(const QString &text);

};

class FortranHighlighter : public Highlighter
{
public:
    FortranHighlighter(QTextDocument *parent = 0);
protected:
    void highlightBlock(const QString &text);

};

#endif
