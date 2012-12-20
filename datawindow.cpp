#include "datawindow.h"
#include "settings.h"
#include <cstdio>


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

    dataTree->addDataItem("n","int","141");
    dataTree->addDataItem("t","double","14.1");
    
    DataItem *d = dataTree->addDataItem("bigStuff", "machine", "" );

    setLayout ( layout );
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
    if ( t == "string" ||
         t == "char" || t == "signed char" || t == "unsigned char" ||
         t == "short" || t == "signed short" || t == "unsigned short" ||
         t == "int" || t == "signed int" || t == "unsigned int" ||
         t == "long" || t == "signed long" || t == "unsigned long" ||
         t == "float" || t == "double" || t == "bool" ) {
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

