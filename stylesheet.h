#ifndef STYLESHEET_H
#define STYLESHEET_H

#include "types.h"

#ifdef STYLESHEET_CPP
StringHash styles;
#else
extern StringHash styles;
#endif

void initStyleSheet(QString key, QString style);
void addStyleSheet(QString key, QString style);

#endif
