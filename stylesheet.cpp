#define STYLESHEET_CPP

#include "stylesheet.h"

#include <QApplication>

void addStyleSheet(QString key, QString style)
{
    styles[key] = style;

    QMap<QString,QString>::const_iterator i = styles.begin();

    QString sheet;
    while ( i != styles.end() ) {
        sheet += i.value();
        i++;
    }
    qApp->setStyleSheet(sheet);
}
