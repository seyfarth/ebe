#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QtGui>

class DataItem : public QTreeWidgetItem
{
public:
    DataItem();
    QString value() {
        return myValue;
    }
    QString name() {
        return myName;
    }
    QString type() {
        return myType;
    }
    void setName(QString n);
    void setValue(QString v);
    void setType(QString t);
    void setRange(int f, int l);
    QString valueFromGdb();

private:
    QString myName;
    QString myValue;
    QString myType;
    int first;
    int last;

public slots:

};

class DataTree : public QTreeWidget
{
    Q_OBJECT

public:
    DataTree(QWidget *parent = 0);
    DataItem *addDataItem(QString n, QString t, QString v);

private:

public slots:
    void expandDataItem(QTreeWidgetItem*);
    void receiveGlobals(QStringList,QStringList,QStringList);
    void receiveLocals(QStringList,QStringList,QStringList);
    void receiveParameters(QStringList,QStringList,QStringList);

};

class DataWindow : public QFrame
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);

private slots:

private:
    QSize sizeHint() const;
    int fontHeight;
    int fontWidth;
};

#endif
