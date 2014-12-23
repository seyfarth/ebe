#include "ebetable.h"

EbeTable::EbeTable(QWidget *parent)
    : QTableWidget(parent)
{
}

void EbeTable::setText ( int r, int c, QString t, Color highlight )
{
    QTableWidgetItem *it = item(r,c);
    QString old = it->text();

    //qDebug() << "setText" << r << c << old << t;
    switch ( highlight ) {
    case Default:
        it->setForeground(QBrush(QColor("black")));
        break;
    case Highlight:
        if ( old != t ) {
            it->setForeground(QBrush(QColor(ebe["next_fg"].toString())));
        } else {
            it->setForeground(QBrush(QColor("black")));
        }
        break;
    case Normal:
        break;
    }
    it->setText(t);
}

EbeTableItem::EbeTableItem ( QString t )
    : QTableWidgetItem(t)
{
}

void EbeTableItem::updateText ( QString t, EbeTable::Color highlight )
{
    QString old = text();
    //qDebug() << old << t;
    //if ( old != "" && old != t ) {
    switch ( highlight ) {
    case EbeTable::Default:
        setForeground(QBrush(QColor("black")));
        break;
    case EbeTable::Highlight:
        if ( old != t ) {
            setForeground(QBrush(QColor(ebe["next_fg"].toString())));
        } else {
            setForeground(QBrush(QColor("black")));
        }
        break;
    case EbeTable::Normal:
        break;
    }
    setText(t);
}
