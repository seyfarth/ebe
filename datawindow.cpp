#include "datawindow.h"
#include "settings.h"
#include "gdb.h"
#include <cstdio>

extern GDB *gdb;

DataTree *dataTree;
DataItem *globals;
DataItem *locals;
DataItem *parameters;
DataItem *userDefined;

static QSet<QString> simpleTypes;

DataWindow::DataWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Data");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(10,10,10,10);

    dataTree = new DataTree(this);
    layout->addWidget(dataTree);

    globals = dataTree->addDataItem("globals","","");
    locals = dataTree->addDataItem("locals","","");
    parameters = dataTree->addDataItem("parameters","","");
    userDefined = dataTree->addDataItem("user-defined","","");
    
    simpleTypes << "char" << "signed char" << "unsigned char"
                << "short" << "signed short" << "unsigned short"
                << "int" << "signed int" << "unsigned int"
                << "long" << "signed long" << "unsigned long"
                << "long int" << "signed long int" << "unsigned long int"
                << "long long" << "signed long long" << "unsigned long long"
                << "bool" << "float" << "double" << "long double"
                << "string";

    setLayout ( layout );

    connect ( gdb, SIGNAL(sendGlobals(QStringList,QStringList,QStringList)),
          dataTree, SLOT(receiveGlobals(QStringList,QStringList,QStringList)));
    connect ( gdb, SIGNAL(sendLocals(QStringList,QStringList,QStringList)),
          dataTree, SLOT(receiveLocals(QStringList,QStringList,QStringList)));
    connect ( gdb, SIGNAL(sendParameters(QStringList,QStringList,QStringList)),
          dataTree, SLOT(receiveParameters(QStringList,QStringList,QStringList)));
}

QSize DataWindow::sizeHint() const
{
    return QSize(300,100);
}

void DataWindow::setFontHeightAndWidth ( int height, int width )
{
}

DataItem::DataItem()
: QTreeWidgetItem()
{
    myName  = "";
    myType  = "";
    myValue = "";
    first   = 0;
    last    = 0;
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
}

QString DataItem::valueFromGdb()
{
    return myValue;
}

void DataItem::setName(QString n)
{
    myName = n;
    if ( first == 0 && last == 0 ) {
        setText(0,myName);
    } else {
        setText(0,QString("%1[%2:%3]").arg(myName).arg(first).arg(last));
    }
}

void DataItem::setType(QString t)
{
    myType = t;
    setText(1,myType);
    if ( simpleTypes.contains(t) ) {
        setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    } else {
        setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    }
}

void DataItem::setValue(QString v)
{
    myValue = v;
    setText(2,myValue);
}

void DataItem::setRange(int f, int l)
{
    first = f;
    last = l;
    setName(myName);
}

DataTree::DataTree(QWidget *parent)
: QTreeWidget(parent)
{
    setColumnCount(3);

    QTreeWidgetItem *header = new QTreeWidgetItem();
    header->setText(0,tr("Name"));
    header->setText(1,tr("Type"));
    header->setText(2,tr("Value"));
    setHeaderItem(header);

    connect ( this, SIGNAL(itemExpanded(QTreeWidgetItem*)),
              this, SLOT(expandDataItem(QTreeWidgetItem*)) );
}

DataItem *DataTree::addDataItem ( QString n, QString t, QString v )
{
    DataItem *d = new DataItem();
    d->setName(n);
    d->setType(t);
    d->setValue(v);
    addTopLevelItem(d);
    return d;
}

void DataTree::expandDataItem(QTreeWidgetItem *item)
{
    DataItem *it = (DataItem *)item;
    qDebug() << "need to expand" << it->name();
    DataItem *d = new DataItem();
    d->setName("a");
    d->setType("int");
    d->setValue("7 8 9 10");
    d->setRange(0,3);
    it->addChild(d);
}

void DataTree::receiveGlobals(QStringList names, QStringList types,
               QStringList values)
{
    int n = names.length();
    DataItem *item;
    
    qDebug() << "rg" << names << types << values;
    globals->takeChildren();
    for ( int i = 0; i < n; i++ ) {
        item = new DataItem();
        item->setName(names[i]);
        item->setType(types[i]);
        item->setValue(values[i]);
        globals->addChild(item);
    }
}

void DataTree::receiveLocals(QStringList names, QStringList types,
               QStringList values)
{
    int n = names.length();
    DataItem *item;
    QList<QTreeWidgetItem*> oldLocals;
    
    oldLocals = locals->takeChildren();
    qDebug() << "rl" << names << types << values;
    for ( int i = 0; i < n; i++ ) {
        qDebug() << i << names[i] << types[i] << values[i];
        item = new DataItem();
        item->setName(names[i]);
        item->setType(types[i]);
        item->setValue(values[i]);
        locals->addChild(item);
    }
    foreach ( QTreeWidgetItem *it, oldLocals ) delete it;
    qDebug() << "done rl";
}

void DataTree::receiveParameters(QStringList names, QStringList types,
               QStringList values)
{
    int n = names.length();
    DataItem *item;
    QList<QTreeWidgetItem*> oldParameters;
    
    oldParameters = parameters->takeChildren();
    qDebug() << "rl" << names << types << values;
    for ( int i = 0; i < n; i++ ) {
        qDebug() << i << names[i] << types[i] << values[i];
        item = new DataItem();
        item->setName(names[i]);
        item->setType(types[i]);
        item->setValue(values[i]);
        parameters->addChild(item);
    }
    foreach ( QTreeWidgetItem *it, oldParameters ) delete it;
    qDebug() << "done rl";
}
