#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QHash>
#include <QString>

#ifdef STYLESHEET_CPP
QHash<QString,QString> styles;
#else
extern QHash<QString,QString> styles;
#endif

void initStyleSheet(QString key, QString style);
void addStyleSheet(QString key, QString style);

#endif
