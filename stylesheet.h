#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QHash>
#include <QString>

#ifndef STYLESHEET_CPP
extern QHash<QString,QString> styles;
#else
QHash<QString,QString> styles;
#endif

void initStyleSheet(QString key, QString style);
void addStyleSheet(QString key, QString style);

#endif
