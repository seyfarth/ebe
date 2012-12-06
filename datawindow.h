#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QSize>
#include <QFrame>
#include <QString>
#include <QStringList>
#include <QTableWidget>

class DataWindow : public QFrame
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);

private slots:

private:
    void addVariable ( QString name, QString value );
    QStringList names;
    QStringList values;
    QTableWidget *table;
    QSize sizeHint() const;
};

#endif
