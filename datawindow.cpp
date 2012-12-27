#include "datawindow.h"
#include "settings.h"
#include "gdb.h"
#include <cstdio>

extern GDB *gdb;
extern QMap<QString,int> sizeForType;
extern DataWindow *dataWindow;

DataTree *dataTree;
DataItem *globals;
DataItem *locals;
DataItem *parameters;
DataItem *userDefined;
QMap<QString,DataItem*> dataMap;

static QSet<QString> simpleTypes;

QMap<QString,ClassDefinition> classes;

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
    globals->setExpanded(true);
    locals = dataTree->addDataItem("locals","","");
    locals->setExpanded(true);
    parameters = dataTree->addDataItem("parameters","","");
    parameters->setExpanded(true);
    userDefined = dataTree->addDataItem("user-defined","","");
    userDefined->setExpanded(true);
    
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
    connect ( dataTree, SIGNAL(requestData(QStringList)),
              gdb, SLOT(getData(QStringList)) );
}

QSize DataWindow::sizeHint() const
{
    return QSize(300,100);
}

void DataWindow::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth  = width;
}

void DataWindow::receiveVariableDefinition(QStringList strings)
{
    DataItem *item;
    int n = userDefined->childCount();
    //qDebug() << "data rec var" << strings;
    QString name = strings[0];
    for ( int i = 0; i < n; i++ ) {
        item = (DataItem *)userDefined->child(i);
        if ( item->name() == name ) {
            item->setAddress(strings[1]);
            item->setFormat(strings[2]);
            item->setSize(strings[3].toInt());
            item->setRange(strings[4].toInt(),strings[5].toInt());
            emit requestData(strings);
            return;
        }
    }
    item = new DataItem;
    item->setUserDefined(true);
    item->setName(name);
    item->setAddress(strings[1]);
    item->setFormat(strings[2]);
    item->setSize(strings[3].toInt());
    item->setRange(strings[4].toInt(),strings[5].toInt());
    userDefined->addChild ( item );
    userDefined->sortChildren ( 0, Qt::AscendingOrder );
    emit requestData(strings);
}

void DataWindow::setData(QStringList strings)
{
    DataItem *item;
    qDebug() << "data rec var" << strings;
    QString name = strings[0];
    item = dataMap[name];
    if ( item->name() == name ) {
        item->setAddress(strings[1]);
        item->setFormat(strings[2]);
        item->setSize(strings[3].toInt());
        item->setRange(strings[4].toInt(),strings[5].toInt());
        item->setValue(strings[6]);
        int n2 = item->childCount();
        for ( int j = 0; j < n2; j++ ) {
            DataItem *it = (DataItem *)item->child(j);
            strings.clear();
            strings.append(it->name());
            strings.append(it->address());
            strings.append(it->format());
            strings.append(QString("%1").arg(it->size()));
            strings.append(QString("%1").arg(it->first()));
            strings.append(QString("%1").arg(it->last()));
            emit requestData(strings);
        }
    }
}

void DataWindow::resetData()
{
    QStringList request;
    DataItem *item;
    QString s;
    int n = userDefined->childCount();
    //qDebug() << "reset" << n;
    for ( int i = 0; i < n; i++ ) {
        item = (DataItem *)userDefined->child(i);
        request.clear();
        request.append(item->myName);
        request.append(item->myAddress);
        request.append(item->myFormat);
        request.append(s.setNum(item->mySize));
        request.append(s.setNum(item->myFirst));
        request.append(s.setNum(item->myLast));
        //qDebug() << "reset req" << i << request;
        emit requestData(request);
    }
}

void DataWindow::receiveClasses(QMap<QString,ClassDefinition> c)
{
    //qDebug() << "receive classes" << c.keys();
    classes = c;
}

DataItem::DataItem()
: QTreeWidgetItem()
{
    myName  = "";
    myType  = "";
    u8 = 0;
    myFirst   = 0;
    myLast    = 0;
    setUserDefined(false);
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
}

QString DataItem::value()
{
    QString val;
    if ( isSimple() ) {
        switch ( mySize ) {
        case 1:
            if ( myFormat == "Character" ) val.sprintf("%c",c1);
            else if ( myFormat == "Decimal" ) val.sprintf("%d",i1);
            else if ( myFormat == "Hexadecimal" ) val.sprintf("%x",u1);
            else if ( myFormat == "Boolean" ) val = b1 ? "true" : "false";
            else val = "";
            break;
        case 2:
            if ( myFormat == "Decimal" ) val.sprintf("%d",i2);
            else if ( myFormat == "Hexadecimal" ) val.sprintf("%x",u2);
            else val = "";
            break;
        case 4:
            if ( myFormat == "Decimal" ) val.sprintf("%d",i4);
            else if ( myFormat == "Hexadecimal" ) val.sprintf("%x",u4);
            else if ( myFormat == "Floating point" ) val.sprintf("%g",f4);
            else val = "";
            break;
        case 8:
            if ( myFormat == "Decimal" ) val.sprintf("%ld",i8);
            else if ( myFormat == "Hexadecimal" ) val.sprintf("%lx",u8);
            else if ( myFormat == "Floating point" ) val.sprintf("%g",f8);
            else val = "";
            break;
        default:
            val = "";
        }
    } else {
        val = stringValue;
    }
    return val;
}
QString DataItem::valueFromGdb()
{
    return "";
}

void DataItem::setName(QString n)
{
    myName = n;
    QString shortName;
    int k = n.lastIndexOf('.');
    if ( n >= 0 ) shortName = n.mid(k+1);
    else shortName = n;
    if ( myFirst == 0 && myLast == 0 ) {
        setText(0,shortName);
    } else {
        setText(0,QString("%1[%2:%3]").arg(shortName).arg(myFirst).arg(myLast));
    }
}

void DataItem::setType(QString t)
{
    myType = t;

    if ( sizeForType.contains(t) ) mySize = sizeForType[t];
    else mySize = 8;

    if ( simpleTypes.contains(t) ) {
        myIsSimple = true;
        if ( t.indexOf("char") >= 0 ) {
            myFormat = "Character";
        } else if ( t.indexOf("short") >= 0 || t.indexOf("int") >= 0 ||
                  t.indexOf("long") >= 0 ) {
            myFormat = "Decimal";
        } else if ( t.indexOf("float") >= 0 || t.indexOf("double") >= 0 ) {
            myFormat = "Floating point";
        } else if ( t.indexOf("bool") >= 0 ) {
            myFormat = "Boolean";
        } else {
            myFormat = "Hexadecimal";
        }
    } else {
        myIsSimple = false;
        myFormat = "";
    }

    setText(1,myType);
    if ( isSimple() ) {
        setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    } else {
        setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    }
}

void DataItem::setValue(QString v)
{
    bool ok;
    if ( isSimple() ) {
        u8 = 0;
        switch ( mySize ) {
        case 1:
            u1 = v.toInt(&ok,16);
            break;
        case 2:
            u2 = v.toInt(&ok,16);
            break;
        case 4:
            u4 = v.toInt(&ok,16);
            break;
        case 8:
            u8 = v.toLong(&ok,16);
            break;
        }
        //qDebug() << myName << mySize << v << u8 << value();
    } else {
        if ( dataMap.contains(name()) ) stringValue = "";
        else stringValue = v;
    }
    setText(2,value());
}

void DataItem::setRange(int f, int l)
{
    myFirst = f;
    myLast = l;
    setName(myName);
}

void DataTree::editUserVariable()
{
}

void DataTree::deleteUserVariable()
{
    QTreeWidgetItem *item = currentItem();
    item->parent()->removeChild(item);
    delete item;
}

void DataTree::contextMenuEvent ( QContextMenuEvent *event )
{
    DataItem *item = (DataItem *)currentItem();
    QString type = item->type();
    //qDebug() << item->name() << type << item->value();
    if ( item->userDefined() ) {
        QMenu menu("Variable menu");
        menu.addAction(tr("Edit variable"),this,SLOT(editUserVariable()));
        menu.addAction(tr("Delete variable"),this,SLOT(deleteUserVariable()));
        menu.exec(QCursor::pos());
    } else if ( item->isSimple() ) {
        if ( type.indexOf("char") >= 0 ) {
            QMenu menu("char menu");
            menu.addAction(tr("Decimal"),this,SLOT(editUserVariable()));
            menu.addAction(tr("Hexadecimal"),this,SLOT(editUserVariable()));
            menu.addAction(tr("Character"),this,SLOT(editUserVariable()));
            menu.exec(QCursor::pos());
        } else if ( type.indexOf("short") >= 0 || type.indexOf("int") >= 0 ||
                    type.indexOf("long") >= 0 ) {
            QMenu menu("Integer menu");
            menu.addAction(tr("Signed decimal"),this,SLOT(editUserVariable()));
            menu.addAction(tr("Unsigned decimal"),this,SLOT(editUserVariable()));
            menu.addAction(tr("Hexadecimal"),this,SLOT(editUserVariable()));
            menu.exec(QCursor::pos());
        }
    }
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

void DataTree::expandDataItem(QTreeWidgetItem *item)
{
    DataItem *it = (DataItem *)item;
    DataItem *d;
    QStringList request;
    QString fullName;
    ClassDefinition c;
    c = classes[it->type()];
    if ( c.name != "" ) {
        foreach ( VariableDefinition v, c.members ) {
            request.clear();
            d = new DataItem;
            fullName = it->name() + "." + v.name;
            d->setName(fullName);
            d->setAddress("&("+fullName+")");
            d->setType(v.type);
            it->addChild(d);
            dataMap[fullName] = d;
            request.append(fullName);
            request.append(d->address());
            request.append(d->format());
            request.append(QString("%1").arg(d->size()));
            request.append(QString("%1").arg(d->first()));
            request.append(QString("%1").arg(d->last()));
            emit requestData(request);
        }
    }
}

void DataTree::receiveGlobals(QStringList names, QStringList types,
               QStringList values)
{
    int n = names.length();
    DataItem *item;
    QStringList request;
    
    //qDebug() << "rg" << names << types << values;
    //globals->takeChildren();
    for ( int i = 0; i < n; i++ ) {
        item = dataMap[names[i]];
        if ( item == 0 ) {
            item = new DataItem();
            item->setName(names[i]);
            item->setType(types[i]);
            item->setValue(values[i]);
            globals->addChild(item);
            dataMap[names[i]] = item;
        } else {
            item->setValue(values[i]);
            int n2 = item->childCount();
            for ( int j = 0; j < n2; j++ ) {
                DataItem *it = (DataItem *)item->child(j);
                request.clear();
                request.append(it->name());
                request.append(it->address());
                request.append(it->format());
                request.append(QString("%1").arg(it->size()));
                request.append(QString("%1").arg(it->first()));
                request.append(QString("%1").arg(it->last()));
                emit requestData(request);
            }
        }
    }
    globals->sortChildren(0,Qt::AscendingOrder);
}

void DataTree::receiveLocals(QStringList names, QStringList types,
               QStringList values)
{
    int n = names.length();
    DataItem *item;
    QStringList request;
    //QList<QTreeWidgetItem*> oldLocals;
    
    //oldLocals = locals->takeChildren();
    //qDebug() << "rl" << names << types << values;
    for ( int i = 0; i < n; i++ ) {
        //qDebug() << i << names[i] << types[i] << values[i];
        item = dataMap[names[i]];
        if ( item == 0 ) {
            item = new DataItem();
            item->setName(names[i]);
            item->setType(types[i]);
            item->setValue(values[i]);
            locals->addChild(item);
            dataMap[names[i]] = item;
        } else {
            item->setValue(values[i]);
            int n2 = item->childCount();
            for ( int j = 0; j < n2; j++ ) {
                DataItem *it = (DataItem *)item->child(j);
                request.clear();
                request.append(it->name());
                request.append(it->address());
                request.append(it->format());
                request.append(QString("%1").arg(it->size()));
                request.append(QString("%1").arg(it->first()));
                request.append(QString("%1").arg(it->last()));
                emit requestData(request);
            }
        }
    }
    globals->sortChildren(0,Qt::AscendingOrder);
}

void DataTree::receiveParameters(QStringList names, QStringList types,
               QStringList values)
{
    int n = names.length();
    DataItem *item;
    QStringList request;
    
    //qDebug() << "rl" << names << types << values;
    for ( int i = 0; i < n; i++ ) {
        //qDebug() << i << names[i] << types[i] << values[i];
        item = dataMap[names[i]];
        if ( item == 0 ) {
            item = new DataItem();
            item->setName(names[i]);
            item->setType(types[i]);
            item->setValue(values[i]);
            parameters->addChild(item);
            dataMap[names[i]] = item;
        } else {
            item->setValue(values[i]);
            int n2 = item->childCount();
            for ( int j = 0; j < n2; j++ ) {
                DataItem *it = (DataItem *)item->child(j);
                request.clear();
                request.append(it->name());
                request.append(it->address());
                request.append(it->format());
                request.append(QString("%1").arg(it->size()));
                request.append(QString("%1").arg(it->first()));
                request.append(QString("%1").arg(it->last()));
                emit requestData(request);
            }
        }
    }
    parameters->sortChildren(0,Qt::AscendingOrder);
    //qDebug() << "done rl";
}
