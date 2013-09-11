#include "types.h"

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

bool operator< ( const FileLine &a, const FileLine &b )
{
    if ( a.file == b.file ) return a.line < b.line;
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

bool operator< ( const FileLabel &a, const FileLabel &b )
{
    if ( a.file == b.file ) return a.label < b.label;
    return a.file < b.file;
}

