#ifndef DEBUG_H
#define DEBUG_H

#include <QtDebug>
extern volatile bool d;
const QDebug &qdebug=qDebug();

#define Debug if(::d) qdebug


#endif
