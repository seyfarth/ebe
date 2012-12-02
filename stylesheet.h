#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QMap>
#include <QString>

#ifndef STYLESHEET_CPP
extern QMap<QString,QString> styles;
#else
QMap<QString,QString> styles;
#endif

void initStyleSheet(QString key, QString style);
void addStyleSheet(QString key, QString style);

#endif
