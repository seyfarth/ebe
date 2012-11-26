#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QScrollArea>
#include <QString>
#include "listwidget.h"

class DataWindow : public QScrollArea
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent=0);
    void setFontWidth(int width);

private slots:

private:
    void addVariable ( QString name, QString value );
    ListWidget *names;
    ListWidget *values;

};

#endif
