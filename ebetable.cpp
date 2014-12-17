#include "ebetable.h"

EbeTable::EbeTable(QWidget *parent)
    : QTableWidget(parent)
{
}

void EbeTable::setText ( int r, int c, QString t, bool highlight )
{
    QTableWidgetItem *it = item(r,c);
    QString old = it->text();
    //qDebug() << old << t;
    if ( highlight && old!= "" && old != t ) {
        it->setForeground(QBrush(QColor(ebe["next_fg"].toString())));
    } else {
        it->setForeground(QBrush(QColor("black")));
    }
    it->setText(t);
}

EbeTableItem::EbeTableItem ( QString t )
    : QTableWidgetItem(t)
{
}

void EbeTableItem::updateText ( QString t, bool highlight )
{
    QString old = text();
    //qDebug() << old << t;
    //if ( old != "" && old != t ) {
    if ( highlight && old != t ) {
        setForeground(QBrush(QColor(ebe["next_fg"].toString())));
    } else {
        setForeground(QBrush(QColor("black")));
    }
    setText(t);
}
