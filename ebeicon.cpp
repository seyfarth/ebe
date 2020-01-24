#include "ebeicon.h"

unsigned short EbeIcon::sizes[] = {16, 24, 32, 48, 64, 80, 96, 112, 128,
                                   160, 192, 224, 256};
unsigned short EbeIcon::n = sizeof(EbeIcon::sizes)/sizeof(unsigned short);

EbeIcon::EbeIcon(QString name)
    : QIcon()
{
    for ( int i = 0; i < n; i++ ) {
        addFile(QString(":/icons/%1/%2").arg(sizes[i]).arg(name),
                        QSize(sizes[i],sizes[i]));
    }
}
