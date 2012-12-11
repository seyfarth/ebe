#define STYLESHEET_CPP

#include "stylesheet.h"

#include <QApplication>
#include <QDebug>

void initStyleSheet(QString key, QString style)
{
    styles[key] = style;
}

void addStyleSheet(QString key, QString style)
{
    styles[key] = style;

    QString sheet;
    foreach ( QString key, styles.keys() ) {
        sheet += styles[key];
    }
    //qDebug() << sheet << endl;
    qApp->setStyleSheet(sheet);
}
