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
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
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

#endif
