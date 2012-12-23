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
    QString address() {
        return myAddress;
    }
    QString format() {
        return myFormat;
    }
    int size() {
        return mySize;
    }
    int first() {
        return myFirst;
    }
    int last() {
        return myLast;
    }
    void setName(QString n);
    void setValue(QString v);
    void setType(QString t);
    void setAddress(QString a);
    void setFormat(QString f);
    void setSize(int s);
    void setRange(int f, int l);
    QString valueFromGdb();

private:
    QString myName;
    QString myValue;
    QString myType;
    QString myAddress;
    QString myFormat;
    int myFirst;
    int myLast;
    int mySize;

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
    void receiveVariableDefinition(QStringList);

private:
    QSize sizeHint() const;
    int fontHeight;
    int fontWidth;
};

#endif
