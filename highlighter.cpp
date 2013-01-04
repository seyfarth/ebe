#include <QtGui>

#include "highlighter.h"
#include "settings.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    keywords << "alignas" << "alignof" << "and" << "and_eq" << "asm"
             << "auto" << "bitand" << "bitor" << "bool" << "break" 
             << "case" << "catch" << "char" << "char16_t" << "char32_t"
             << "class" << "compl" << "const" << "constexpr" 
             << "continue" << "decltype" << "default" << "delete"
             << "do" << "double" << "dynamic_cast" << "else" << "enum"
             << "explicit" << "export" << "extern" << "false" 
             << "float" << "for" << "friend" << "goto" << "inline"
             << "int" << "long" << "mutable" << "namespace" << "new"
             << "noexcept" << "not" << "not_eq" << "NULL" << "nullptr"
             << "operator" << "or" << "or_eq" << "private" << "protected"
             << "public" << "register" << "reinterpret_cast" << "return"
             << "short" << "signed" << "sizeof" << "static"
             << "static_assert" << "static_cast" << "struct" << "switch"
             << "template" << "this" << "thread_local" << "throw" 
             << "true" << "try" << "typedef" << "typeid" << "typename"
             << "union" << "unsigned" << "using" << "virtual" << "void"
             << "volatile" << "wchar_t" << "while" << "xor" << "xor_eq";

    operators = QString("!$%^&*()-+=|{}[]\"':;?/<,>.");
    keywordFormat.setForeground(QColor(ebe["reserved_fg"].toString()));
    idFormat.setForeground(QColor(ebe["id_fg"].toString()));
    numberFormat.setForeground(QColor(ebe["numeric_fg"].toString()));
    commentFormat.setForeground(QColor(ebe["comment_fg"].toString()));
    stringFormat.setForeground(QColor(ebe["string_fg"].toString()));
    operatorFormat.setForeground(QColor(ebe["operator_fg"].toString()));
    preprocessorFormat.setForeground(QColor(ebe["preprocessor_fg"].toString()));
    illegalFormat.setForeground(QColor(ebe["illegal_fg"].toString()));
    illegalFormat.setBackground(QColor(ebe["illegal_bg"].toString()));
}

void Highlighter::highlightBlock(const QString &text)
{
    int i = 0;
    int n = text.length();
    int start;
    int end;
    QChar startChar;
    QString name;
    QString t = text + "@@'\"";

    if ( n == 0 ) {
        setCurrentBlockState(previousBlockState());
        return;
    }

    if ( previousBlockState() == 1 ) goto CComment;

    More:
        if ( i >= n ) return;
        setCurrentBlockState(0);
        if ( t[i].isLetter() ) goto Identifier;
        if ( t[i].isDigit() ) goto Number;
        if ( t[i] == QChar('#') ) goto Preprocessor;
        if ( t[i] == QChar('/') &&
             t[i+1] == QChar('/') ) goto CPPComment;
        if ( t[i] == QChar('/') &&
             t[i+1] == QChar('*') ) goto CComment;
        if ( t[i].isSpace() ) goto Space;
        if ( t[i] == QChar('"') || t[i] == QChar('\'') ) goto String;
        if ( operators.contains(t[i]) ) goto Operator;
        goto  Illegal;

    Identifier:
        start = i;
    IdentifierMore:
        i++;
        if ( t[i].isLetterOrNumber() ) goto IdentifierMore;
        if ( t[i] == QChar('_') ) goto IdentifierMore;
        end = i-1;
        name = t.mid(start,end-start+1);
        if ( keywords.contains(name) ) {
            setFormat(start,end-start+1,keywordFormat);
        } else {
            setFormat(start,end-start+1,idFormat);
        }
        goto More;
        

    Number:
        start = i;
    NumberMore:
        i++;
        if ( t[i].isDigit() ) goto NumberMore;
        if ( t[i] == QChar('.') ) goto NumberMore;
        end = i-1;
        setFormat(start,end-start+1,numberFormat);
        goto More;

    String:
        start = i;
        startChar = t[i];
    StringMore:
        i++;
        if ( t[i] == QChar('\\') ) {
            i++;
            goto StringMore;
        }
        if ( t[i] != startChar ) goto StringMore;
        end = i-1;
        i++;
        setFormat(start,end-start+1,stringFormat);
        goto More;


    CPPComment:
        start = i;
        end = n-1;
        setFormat(start,end-start+1,commentFormat);
        return;

    CComment:
        start = i;
        setCurrentBlockState(1);
    CCommentMore:
        if ( i >= n ) {
            end = n-1;
            setFormat(start,end-start+1,commentFormat);
            return;
        }
        if ( t[i] != QChar('*')  ) {
            i++;
            goto CCommentMore;
        }
        i++;
        if ( t[i] != QChar('/') ) {
            i++;
            goto CCommentMore;
        }
        end = i;
        setFormat(start,end-start+1,commentFormat);
        setCurrentBlockState(0);
        i++;
        goto More;

    Illegal:
        setFormat(i,1,illegalFormat);
        i++;
        goto More;

    Operator:
        setFormat(i,1,operatorFormat);
        i++;
        goto More;

    Preprocessor:
        start = i;
        end = n-1;
        setFormat(start,end-start+1,preprocessorFormat);
        return;

    Space:
        i++;
        goto More;
}
