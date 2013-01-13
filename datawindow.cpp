#include "datawindow.h"
#include "settings.h"
#include "backtracewindow.h"
#include "gdb.h"
#include <cstdio>

extern GDB *gdb;
extern BackTraceWindow *backTraceWindow;
extern QMap<QString,int> sizeForType;
extern DataWindow *dataWindow;

DataTree *dataTree;
DataItem *globals;
DataItem *locals;
DataItem *parameters;
DataItem *userDefined;
DataMap *localMap;
DataMap *parameterMap;
DataMap *globalMap;
DataMap *userDefinedMap;

QStack<DataTree*> stack;

static QSet<QString> simpleTypes;

QMap<QString,ClassDefinition> classes;

DataWindow::DataWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Data");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    layout = new QVBoxLayout();
    layout->setContentsMargins(10,10,10,10);

    dataTree = new DataTree(this);
    layout->addWidget(dataTree);
    dataTree->setToolTip(
        tr("Left click on a > symbol to expand an item.\n"
           "Right click to change formats."));

    level = 1;

    simpleTypes << "char" << "signed char" << "unsigned char"
                << "short" << "signed short" << "unsigned short"
                << "int" << "signed int" << "unsigned int"
                << "long" << "signed long" << "unsigned long"
                << "long int" << "signed long int" << "unsigned long int"
                << "long long" << "signed long long" << "unsigned long long"
                << "bool" << "float" << "double" << "long double"
                << "string";

    setLayout ( layout );

    qRegisterMetaType<DataMap*>("DataMap*");

    connect ( gdb, SIGNAL(sendGlobals(QStringList,QStringList,QStringList)),
          this, SLOT(receiveGlobals(QStringList,QStringList,QStringList)));
    connect ( gdb, SIGNAL(sendLocals(QStringList,QStringList,QStringList)),
          this, SLOT(receiveLocals(QStringList,QStringList,QStringList)));
    connect ( gdb, SIGNAL(sendParameters(QStringList,QStringList,QStringList)),
         this, SLOT(receiveParameters(QStringList,QStringList,QStringList)));
    connect (
       this, SIGNAL(requestVar(DataMap*,QString,QString,QString,
                        QString,int,int,int)),
       gdb, SLOT(requestVar(DataMap*,QString,QString,QString,
                 QString,int,int,int)) );
    connect ( gdb, SIGNAL(sendVar(DataMap*,QString,QString)),
              this, SLOT(receiveVar(DataMap*,QString,QString)) );
    //connect ( gdb, SIGNAL(dataReady(QStringList)),
              //this, SLOT(setData(QStringList)) );
    connect ( gdb, SIGNAL(resetData()),
              this, SLOT(resetData()) );
    connect ( gdb, SIGNAL(sendClasses(QMap<QString,ClassDefinition>)),
              this, SLOT(receiveClasses(QMap<QString,ClassDefinition>)) );
}

QSize DataWindow::sizeHint() const
{
    return QSize(300,50);
}

void DataWindow::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth  = width;
}

void DataWindow::receiveVariableDefinition(QStringList strings)
{
    DataItem *item;
    //qDebug() << "data rec var" << strings;
    QString name = strings[0];
    item = userDefinedMap->value(name);
    if ( item == 0 ) {
        item = dataTree->addDataItem(userDefinedMap,name,strings[1],"");
        item->format=strings[2];
        item->size=strings[3].toInt();
        item->setRange(strings[4].toInt(),strings[5].toInt());
        userDefined->addChild ( item );
        userDefined->sortChildren ( 0, Qt::AscendingOrder );
    }
    request(item);
}

void DataWindow::resetData()
{
    QString s;
    int n = userDefined->childCount();
    //qDebug() << "reset" << n;
    for ( int i = 0; i < n; i++ ) {
        request((DataItem *)userDefined->child(i));
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
    name  = "";
    type  = "";
    u8    = 0;
    first = 0;
    last  = 0;
    userDefined = false;
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
}

QString DataItem::value()
{
    QString val;
    if ( isSimple ) {
        switch ( size ) {
        case 1:
            if ( format == "Character" ) val.sprintf("%c",c1);
            else if ( format == "Decimal" ) val.sprintf("%d",i1);
            else if ( format == "Unsigned decimal" ) val.sprintf("%d",u1);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%x",u1);
            else if ( format == "Boolean" ) val = b1 ? "true" : "false";
            else val = "";
            break;
        case 2:
            if ( format == "Decimal" ) val.sprintf("%d",i2);
            else if ( format == "Unsigned decimal" ) val.sprintf("%d",u2);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%x",u2);
            else val = "";
            break;
        case 4:
            if ( format == "Decimal" ) val.sprintf("%d",i4);
            else if ( format == "Unsigned decimal" ) val.sprintf("%d",u4);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%x",u4);
            else if ( format == "Floating point" ) val.sprintf("%g",f4);
            else val = "";
            break;
        case 8:
            if ( format == "Decimal" ) val.sprintf("%ld",i8);
            else if ( format == "Unsigned decimal" ) val.sprintf("%ld",u8);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%lx",u8);
            else if ( format == "Floating point" ) val.sprintf("%g",f8);
            else val = "";
            break;
        default:
            val = "";
        }
    } else if ( classes.contains(type) ) {
        val = "";
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
    name = n;
    QString shortName;
    int k = n.lastIndexOf('.');
    if ( n >= 0 ) shortName = n.mid(k+1);
    else shortName = n;
    if ( first == 0 && last == 0 ) {
        setText(0,shortName);
    } else {
        setText(0,QString("%1[%2:%3]").arg(shortName).arg(first).arg(last));
    }
}

void DataItem::setType(QString t)
{
    type = t;

    if ( sizeForType.contains(t) ) size = sizeForType[t];
    else size = 8;

    if ( simpleTypes.contains(t) ) {
        isSimple = true;
        if ( t.indexOf("char") >= 0 ) {
            format = "Character";
        } else if ( t.indexOf("short") >= 0 || t.indexOf("int") >= 0 ||
                  t.indexOf("long") >= 0 ) {
            format = "Decimal";
        } else if ( t.indexOf("float") >= 0 || t.indexOf("double") >= 0 ) {
            format = "Floating point";
        } else if ( t.indexOf("bool") >= 0 ) {
            format = "Boolean";
        } else {
            format = "Hexadecimal";
        }
    } else {
        isSimple = false;
        if ( t.indexOf(" *") >= 0 ) format = "Pointer";
        else format = "";
    }

    setText(1,type);
    if ( isSimple ) {
        setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    } else {
        setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    }
}

void DataItem::setValue(QString v)
{
    bool ok;

    stringValue = v;
    if ( isSimple ) {
        u8 = 0;
        switch ( size ) {
        case 1:
            u1 = v.toUInt(&ok,16);
            break;
        case 2:
            u2 = v.toUInt(&ok,16);
            break;
        case 4:
            u4 = v.toUInt(&ok,16);
            break;
        case 8:
#ifdef Q_WS_WIN
            u8 = v.toULongLong(&ok,16);
#else
            u8 = v.toULong(&ok,16);
#endif
            break;
        }
        //qDebug() << name << size << v << u8 << value();
    }
    setText(2,value());
}

void DataItem::setRange(int f, int l)
{
    first = f;
    last = l;
    setName(name);
}

void DataTree::setDecimal()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Decimal";
    d->setText(2,d->value());
}

void DataTree::setUnsignedDecimal()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Unsigned decimal";
    d->setText(2,d->value());
}

void DataTree::setHexadecimal()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Hexadecimal";
    d->setText(2,d->value());
}

void DataTree::setCharacter()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Character";
    d->setText(2,d->value());
}

void DataTree::editUserVariable()
{
    DataItem *d = (DataItem *)currentItem();
    DefineVariableDialog *dialog = new DefineVariableDialog;
    dialog->nameEdit->setText(d->name);
    dialog->addressEdit->setText(d->address);
    dialog->formatCombo->setCurrentIndex(dialog->formatCombo->findText(d->format));
    dialog->sizeCombo->setCurrentIndex(dialog->sizeCombo->findText(QString("%1").arg(d->size)));
    dialog->firstEdit->setText(QString("%1").arg(d->first));
    dialog->lastEdit->setText(QString("%1").arg(d->last));
    if ( dialog->exec() ) {
        userDefinedMap->remove(d->name);
        d->setName(dialog->nameEdit->text());
        d->address = dialog->addressEdit->text();
        d->format = dialog->formatCombo->currentText();
        d->size = dialog->sizeCombo->currentText().toInt();
        d->first = dialog->firstEdit->text().toInt();
        d->last = dialog->lastEdit->text().toInt();
        userDefinedMap->insert(d->name,d);
        d->setText(2,d->value());
    }
    delete dialog;
}

void DataTree::deleteUserVariable()
{
    DataItem *item = (DataItem *)currentItem();
    item->parent()->removeChild(item);
    userDefinedMap->remove(item->name);
    delete item;
}

void DataTree::contextMenuEvent ( QContextMenuEvent * /*event*/ )
{
    DataItem *item = (DataItem *)currentItem();
    QString type = item->type;
    //qDebug() << item->name << type << item->value();
    if ( item->userDefined ) {
        QMenu menu("Variable menu");
        menu.addAction(tr("Edit variable"),this,SLOT(editUserVariable()));
        menu.addAction(tr("Delete variable"),this,SLOT(deleteUserVariable()));
        menu.exec(QCursor::pos());
    } else if ( item->isSimple ) {
        if ( type.indexOf("char") >= 0 ) {
            QMenu menu("char menu");
            menu.addAction(tr("Decimal"),this,SLOT(setDecimal()));
            menu.addAction(tr("Unsigned decimal"),this,SLOT(setUnsignedDecimal()));
            menu.addAction(tr("Hexadecimal"),this,SLOT(setHexadecimal()));
            menu.addAction(tr("Character"),this,SLOT(setCharacter()));
            menu.exec(QCursor::pos());
        } else if ( type.indexOf("short") >= 0 || type.indexOf("int") >= 0 ||
                    type.indexOf("long") >= 0 ) {
            QMenu menu("Integer menu");
            menu.addAction(tr("Signed decimal"),this,SLOT(setDecimal()));
            menu.addAction(tr("Unsigned decimal"),this,SLOT(setUnsignedDecimal()));
            menu.addAction(tr("Hexadecimal"),this,SLOT(setHexadecimal()));
            menu.exec(QCursor::pos());
        }
    }
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

    ::globalMap = globalMap = new DataMap;
    ::localMap = localMap  = new DataMap;
    ::parameterMap = parameterMap = new DataMap;
    ::userDefinedMap = userDefinedMap = new DataMap;

    ::globals = globals = addDataItem(globalMap,"globals","","");
    ::locals = locals = addDataItem(localMap,"locals","","");
    ::parameters = parameters = addDataItem(parameterMap,"parameters","","");
    ::userDefined = userDefined = addDataItem(userDefinedMap,"user-defined","","");

    addTopLevelItem(globals);
    globals->setExpanded(true);
    addTopLevelItem(locals);
    locals->setExpanded(true);
    addTopLevelItem(parameters);
    parameters->setExpanded(true);
    addTopLevelItem(userDefined);
    userDefined->setExpanded(true);
    
    connect ( this, SIGNAL(itemExpanded(QTreeWidgetItem*)),
              this, SLOT(expandDataItem(QTreeWidgetItem*)) );
}

DataTree::~DataTree()
{
    globals->removeSubTree();
    delete globals;
    locals->removeSubTree();
    delete locals;
    parameters->removeSubTree();
    delete parameters;
    userDefined->removeSubTree();
    delete userDefined;
    delete globalMap;
    delete localMap;
    delete parameterMap;
    delete userDefinedMap;
}
void DataTree::expandDataItem(QTreeWidgetItem *item)
{
    DataItem *it = (DataItem *)item;
    DataItem *d;
    QString fullName;
    ClassDefinition c;
    int n;
    c = classes[it->type];
    QList<QTreeWidgetItem*> kids = it->takeChildren();
    foreach ( QTreeWidgetItem *t, kids ) {
        ((DataItem *)t)->removeSubTree();
        delete t;
    }
    if ( c.name != "" ) {
        foreach ( VariableDefinition v, c.members ) {
            fullName = it->name + "." + v.name;
            d = addDataItem(it->map,fullName,v.type,"");
            if ( it->map == globalMap ) {
                d->address="&(::"+fullName+")";
            } else {
                d->address="&("+fullName+")";
            }
            it->addChild(d);
            dataWindow->request(d);
        }
    } else if ( (n = it->type.indexOf(" *")) > 0 ) {
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        if ( dialog->exec() ) {
            int min = dialog->min;
            int max = dialog->max;
            QString type = it->type;
            type.remove(n+1,1);
            n = type.length();
            if ( type[n-1] == ' ' ) type.chop(1);
            for ( int i = min; i <= max; i++ ) {
                fullName = it->name+QString("[%1]").arg(i);
                d = addDataItem(it->map,fullName,type,"");
                d->address="&("+fullName+")";
                it->addChild(d);
                dataWindow->request(d);
            }
        }
        delete dialog;
    } else if ( (n = it->type.indexOf('[')) >= 0 ) {
        QString type = it->type;
        int n2 = type.indexOf(']');
        if ( n2 < 0 ) return;
        QString dimString = type.mid(n+1,n2-n-1);
        type.remove(n,n2-n+1);
        n = type.length();
        if ( type[n-1] == ' ' ) type.chop(1);
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        int dim;
        if ( dimString.length() > 0 ) {
            dim = dimString.toInt();
            dialog->setMax(dim-1);
        }
        if ( dialog->exec() ) {
            int min = dialog->min;
            int max = dialog->max;
            int n3 = type.indexOf('[');
            int n4 = type.indexOf(']');
            int dim2;
            if ( n3 > 0 && n4 > 0 ) {
                dimString = type.mid(n3+1,n4-n3-1);
                dim2 = dimString.toInt() - 1;
                if ( dim2 > 9 ) dim2 = 9;
            } else {
                dim2 = 0;
            }
            int isize = 8;
            QString format;
            if ( type.indexOf('[',n4) >= 0 ) {
                dim2 = 0;
            } else {
                QString newType = type.left(n3-1);
                if ( simpleTypes.contains(newType) ) {
                    isize = sizeForType[newType];
                    if ( newType == "char" ) {
                        format = "Character";
                    } else if ( newType == "float" || newType == "double" ) {
                        format = "Floating point";
                    } else if ( newType == "bool" ) {
                        format = "Boolean";
                    } else {
                        format = "Decimal";
                    }
                }
            }
            for ( int i = min; i <= max; i++ ) {
                fullName = it->name+QString("[%1]").arg(i);
                d = addDataItem(it->map,fullName,type,"");
                d->size = isize;
                d->last = dim2;
                d->format = format;
                it->addChild(d);
                dataWindow->request(d);
            }
        }
    }
}

void DataWindow::request(DataItem *d)
{
    emit requestVar(d->map,d->name,d->address,d->type,
                    d->format,d->size,d->first,d->last);
}

void DataWindow::receiveVar(DataMap *map, QString name, QString value)
{
    DataItem *item;
    //qDebug() << "rv" << name << value;
    item = map->value(name);
    if ( item == 0 ) {
        return;
    }
    item->setValue(value);
    int n2 = item->childCount();
    for ( int j = 0; j < n2; j++ ) {
        request((DataItem *)item->child(j));
    }
}

void DataWindow::receiveGlobals(QStringList names, QStringList types,
               QStringList values)
{
    int n = names.length();
    DataItem *item;
    
    for ( int i = 0; i < n; i++ ) {
        item = globalMap->value(names[i]);
        if ( item == 0 ) {
            item = dataTree->addDataItem(globalMap,names[i],types[i],values[i]);
            item->address = QString("&(::%1").arg(names[i]);
            globals->addChild(item);
        } else {
            item->setValue(values[i]);
            int n2 = item->childCount();
            for ( int j = 0; j < n2; j++ ) {
                request((DataItem *)item->child(j));
            }
        }
    }
    globals->sortChildren(0,Qt::AscendingOrder);
}

void DataItem::removeSubTree()
{
    int i;
    int n = childCount();
    DataItem *item;
    QList<QTreeWidgetItem*> kids;

    for ( i = 0; i < n; i++ ) {
        item = (DataItem *)child(i);
        item->removeSubTree();
    }
    kids = takeChildren();
    for ( i = 0; i < n; i++ ) {
        item = (DataItem *)kids[i];
        item->map->remove(item->name);
        delete item;
    }
}

void DataWindow::receiveLocals(QStringList names, QStringList types, QStringList values)
{
    int n = names.length();
    DataItem *item;
    
    while ( level < backTraceWindow->level ) {
        dataTree->hide();
        stack.push(dataTree);
        dataTree = new DataTree;
        layout->addWidget(dataTree);
        level++;
    }
    while ( level > backTraceWindow->level ) {
        dataTree->hide();
        layout->removeWidget(dataTree);
        delete dataTree;
        dataTree = stack.pop();
        dataTree->show();
        globals = dataTree->globals;
        locals = dataTree->locals;
        parameters = dataTree->parameters;
        userDefined = dataTree->userDefined;
        globalMap = dataTree->globalMap;
        localMap = dataTree->localMap;
        parameterMap = dataTree->parameterMap;
        userDefinedMap = dataTree->userDefinedMap;
        level--;
    }
    for ( int i = 0; i < n; i++ ) {
        item = localMap->value(names[i]);
        if ( item == 0 ) {
            item = dataTree->addDataItem(localMap,names[i],types[i],values[i]);
            item->address = QString("&(%1)").arg(names[i]);
            locals->addChild(item);
        } else {
            item->setValue(values[i]);
            int n2 = item->childCount();
            for ( int j = 0; j < n2; j++ ) {
                request((DataItem *)item->child(j));
            }
        }
    }
    locals->sortChildren(0,Qt::AscendingOrder);
}

void DataWindow::receiveParameters(QStringList names, QStringList types,
                                 QStringList values)
{
    int n = names.length();
    DataItem *item;
    
    for ( int i = 0; i < n; i++ ) {
        //qDebug() << i << names[i] << types[i] << values[i];
        item = parameterMap->value(names[i]);
        if ( item == 0 ) {
            item = dataTree->addDataItem(parameterMap,names[i],types[i],values[i]);
            item->address = QString("&(%1)").arg(names[i]);
            parameters->addChild(item);
        } else {
            item->setValue(values[i]);
            int n2 = item->childCount();
            for ( int j = 0; j < n2; j++ ) {
                request((DataItem *)item->child(j));
            }
        }
    }
    parameters->sortChildren(0,Qt::AscendingOrder);
    //qDebug() << "done rl";
}

DataItem *DataTree::addDataItem ( DataMap *map, QString n,
                                  QString t, QString v )
{
    DataItem *d = new DataItem;
    d->setName(n);
    d->setType(t);
    d->setValue(v);
    d->userDefined = (map == userDefinedMap);
    if ( map == globalMap ) {
        d->address = QString("&(::%1)").arg(n);
    } else {
        d->address = QString("&(%1)").arg(n);
    }
    d->map = map;
    map->insert(n,d);
    return d;
}
