#ifndef EBEICON_H
#define EBEICON_H

#include <QString>
#include <QIcon>

class EbeIcon: public QIcon
{

    static unsigned short sizes[];
    static unsigned short n;
public:
    EbeIcon(QString name);
};

#endif
