#ifndef EBETABLE_H
#define EBETABLE_H

#include "types.h"
#include "settings.h"
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>

class EbeTable: public QTableWidget
{
    Q_OBJECT

public:
    EbeTable(QWidget *parent = 0);
    void setText ( int r, int c, QString t, bool highlight=false );
};

class EbeTableItem: public QTableWidgetItem
{
public:
    EbeTableItem(QString t);
    void updateText(QString t, bool highlight=false);
};
#endif

