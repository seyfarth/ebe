#ifndef BITBUCKET_H
#define BITBUCKET_H

#include <QtGui>
#include "types.h"
#include "unarybitpanel.h"
#include "binarybitpanel.h"

class CommandLine;

class BitBucket : public QFrame
{
    Q_OBJECT

public:
    BitBucket(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    int  fontHeight;
    int  fontWidth;

    UnaryBitPanel *unaryPanel;
    BinaryBitPanel *binaryPanel;

public slots:

signals:

private:
    QTabWidget *tab;
};

#endif
