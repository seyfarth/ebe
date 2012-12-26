#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QtGui>

class DataItem : public QTreeWidgetItem
{
public:
    DataItem();
    QString value();
    QString name() { return myName; }
    QString type() { return myType; }
    QString address() { return myAddress; }
    QString format() { return myFormat; }
    bool isSimple() { return myIsSimple; }
    int size() { return mySize; }
    int first() { return myFirst; }
    int last() { return myLast; }
    bool userDefined() { return myUserDefined; }
    void setName(QString n);
    void setValue(QString v);
    void setType(QString t);
    void setAddress(QString a) { myAddress = a; }
    void setFormat(QString f) { myFormat = f; }
    void setSize(int s) { mySize = s; }
    void setFirst(int f) { myFirst = f; }
    void setLast(int l) { myLast = l; }
    void setRange(int f, int l);
    void setUserDefined(bool u) { myUserDefined = u; }
    void setIsSimple ( bool i ) { myIsSimple = i; }
    QString valueFromGdb();

    QString myName;
    QString myType;
    QString myAddress;
    QString myFormat;
    int myFirst;
    int myLast;
    int mySize;
    bool myUserDefined;
    bool myIsSimple;
    union {
        double f8;
        float f4;
        unsigned long u8;
        unsigned int u4;
        unsigned short u2;
        unsigned char u1;
        long i8;
        int i4;
        short i2;
        signed char i1;
        char c1;
        bool b1;
    };
    QString stringValue;
public slots:

};

class DataTree : public QTreeWidget
{
    Q_OBJECT

public:
    DataTree(QWidget *parent = 0);
    DataItem *addDataItem(QString n, QString t, QString v);
    void contextMenuEvent(QContextMenuEvent *event);

private:

public slots:
    void expandDataItem(QTreeWidgetItem*);
    void receiveGlobals(QStringList,QStringList,QStringList);
    void receiveLocals(QStringList,QStringList,QStringList);
    void receiveParameters(QStringList,QStringList,QStringList);
    void editUserVariable();
    void deleteUserVariable();

};

class DataWindow : public QFrame
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);

private slots:
    void receiveVariableDefinition(QStringList);
    void setData(QStringList);
    void resetData();

private:
    QSize sizeHint() const;
    int fontHeight;
    int fontWidth;

signals:
    void requestData(QStringList);
};

#endif
