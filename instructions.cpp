#include <QFile>
#include <QDebug>
#include "types.h"

StringSet instructions;

void readInstructions()
{
    QFile file(QString(":/src/assembly/instructions"));
    if (file.open(QFile::ReadOnly)) {
        QString t;
        t = file.readLine();
        while (t != "") {
            t = t.trimmed();
            instructions.insert(t);
            t = file.readLine();
        }
        file.close();
    } else {
        qDebug() << QFile::tr("Could not open :/src/assembly/instructions");
    }
}
