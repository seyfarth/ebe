#ifndef EBETABLE_H
#define EBETABLE_H

#include "types.h"
#include "settings.h"
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>

class EbeTable: public QTableWidget
{
    Q_OBJECT

public:
    enum Color { Default, Normal, Highlight };
    EbeTable(QWidget *parent = 0);
    void setText ( int r, int c, QString t, Color highlight=Default );
};

class EbeTableItem: public QTableWidgetItem
{
public:
    EbeTableItem(QString t);
    void updateText(QString t, EbeTable::Color highlight=EbeTable::Default);
};
#endif

