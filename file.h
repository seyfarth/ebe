#ifndef FILE_H
#define FILE_H

#include "types.h"

class File
{
public:
    File();
    QString base;
    QString source;
    QString object;
    QString ext;
    QString language;
    int lineCount;
    void setLanguage();
};

#endif
