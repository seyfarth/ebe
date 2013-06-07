#include "file.h"
#include <QStringList>

extern QStringList cppExts;
extern QStringList cExts;
extern QStringList fortranExts;
extern QStringList asmExts;

File::File()
{
    source = object = base = ext = language = simple = "";
}

void File::setLanguage()
{
    int n = source.lastIndexOf('.');
    if ( n > 0 ) {
        ext = source.mid(n+1);
        if ( cppExts.contains(ext) ) language = "cpp";
        else if ( cExts.contains(ext) ) language = "c";
        else if ( asmExts.contains(ext) ) language = "asm";
        else if ( fortranExts.contains(ext) ) language = "fortran";
        base = source.left(n);
        object = base + ".o";
    } else {
        language = "";
        ext = "";
        object = "";
        base = source;
    }

    n = source.lastIndexOf('/');
    if ( n > 0 ) simple = source.mid(n+1);
    else simple = source;
}
