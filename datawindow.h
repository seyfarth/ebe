#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QtGui>
#include <variable.h>

class DataGroup;

class DataItem : public QTreeWidgetItem
{
public:
    DataItem();
    QString value();
    QString name;
    QString type;
    QString address;
    QString format;
    bool isSimple;
    int size;
    int first;
    int last;
    bool userDefined;
    void setName(QString n);
    void setValue(QString v);
    void setType(QString t);
    void setRange(int f, int l);
    void removeSubTree();
    QString valueFromGdb();
    QMap<QString,DataItem*> *map;
    QString stringValue;

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

};

typedef QMap<QString,DataItem*> DataMap;

class DataTree : public QTreeWidget
{
    Q_OBJECT

public:
    DataTree(QWidget *parent = 0);
    ~DataTree();
    void contextMenuEvent(QContextMenuEvent *event);
    DataItem *globals;
    DataItem *locals;
    DataItem *parameters;
    DataItem *userDefined;
    DataMap *globalMap;
    DataMap *localMap;
    DataMap *parameterMap;
    DataMap *userDefinedMap;
    DataItem *addDataItem(DataMap *map, QString n,
                          QString t, QString v);


public slots:
    void expandDataItem(QTreeWidgetItem*);
    void editUserVariable();
    void deleteUserVariable();
    void setDecimal();
    void setCharacter();
    void setUnsignedDecimal();
    void setHexadecimal();

private:

signals:
};

class DataWindow : public QFrame
{
    Q_OBJECT

public:
    DataWindow(QWidget *parent=0);
    void setFontHeightAndWidth(int height, int width);
    void request(DataItem *item);
    int level;
    QVBoxLayout *layout;

private slots:
    void receiveVariableDefinition(QStringList);
    void resetData();
    void receiveClasses(QMap<QString,ClassDefinition> c);
    void receiveVar(DataMap *group, QString name, QString value);
    void receiveGlobals(QStringList,QStringList,QStringList);
    void receiveLocals(QStringList,QStringList,QStringList);
    void receiveParameters(QStringList,QStringList,QStringList);

private:
    QSize sizeHint() const;
    int fontHeight;
    int fontWidth;

signals:
    void requestVar(DataMap*,QString,QString,QString,QString,int,int,int);
};

#endif
