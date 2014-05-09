#include "datawindow.h"
#include "settings.h"
#include "backtracewindow.h"
#include "gdb.h"
#include "types.h"
#include <cstdio>
#include <QMenu>

extern GDB *gdb;
extern BackTraceWindow *backTraceWindow;
extern IntHash sizeForType;
extern QSet<QString> simpleTypes;
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

extern QHash<QString,Range> labelRange;
QStack<DataTree*> stack;

extern StringHash varToAddress;

QHash<QString,ClassDefinition> classes;

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

    setLayout ( layout );

    qRegisterMetaType<DataMap*>("DataMap*");
    qRegisterMetaType<QList<VariableDefinition> >("QList<VariableDefinition>");

    connect ( gdb, SIGNAL(sendGlobals(QList<VariableDefinition>)),
          this, SLOT(receiveGlobals(QList<VariableDefinition>)));
    connect ( gdb, SIGNAL(sendLocals(QList<VariableDefinition>)),
          this, SLOT(receiveLocals(QList<VariableDefinition>)));
    connect ( gdb, SIGNAL(sendParameters(QList<VariableDefinition>)),
         this, SLOT(receiveParameters(QList<VariableDefinition>)));
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
    connect ( gdb, SIGNAL(sendClasses(QHash<QString,ClassDefinition>)),
              this, SLOT(receiveClasses(QHash<QString,ClassDefinition>)) );
}

QSize DataWindow::sizeHint() const
{
    return QSize(200,10);
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
    int line = strings[7].toInt();
    Range range;
    if ( name[0] == QChar('.') ) {
        foreach ( QString label, labelRange.keys() ) {
            range = labelRange[label];
            //qDebug() << label << range.first << range.last;
            if ( line >= range.first && line <= range.last &&
                 varToAddress.contains(label+name) ) {
                name = label+name;
                break;
            }
        }
    }
    item = userDefinedMap->value(name);
    if ( item == 0 ) {
        item = dataTree->addDataItem(userDefinedMap,name,strings[2],"");
        item->address = strings[1];
        if ( varToAddress.contains(name) ) {
            item->address = varToAddress[name];
        }
        if ( item->address == "" ) {
            item->address = QString("&(%1)").arg(name);
        }
        if ( strings[3] == "String" && strings[5] == "0" ) strings[5] = "1";
        item->setRange(strings[4].toInt(),strings[5].toInt());
        item->setType(strings[2]);
        item->format=strings[3];
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

void DataWindow::receiveClasses(QHash<QString,ClassDefinition> c)
{
    //qDebug() << "receive classes" << c.keys();
    classes = c;
}

DataItem::DataItem()
: QTreeWidgetItem()
{
    name  = "";
    type  = "";
    a.u8    = 0;
    first = 0;
    last  = 0;
    userDefined = false;
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
}

QString DataItem::value()
{
    QString val;
    if ( isSimple && last == 0 ) {
        switch ( size ) {
        case 1:
            if ( format == "Character" ) val.sprintf("%c",a.c);
            else if ( format == "Decimal" ) val.sprintf("%d",a.i1);
            else if ( format == "Unsigned decimal" ) val.sprintf("%d",a.u1);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%x",a.u1);
            else if ( format == "Boolean" ) val = a.b1 ? "true" : "false";
            else if ( format == "Binary" ) val = binary(a,1);
            else val = "";
            break;
        case 2:
            if ( format == "Decimal" ) val.sprintf("%d",a.i2);
            else if ( format == "Unsigned decimal" ) val.sprintf("%d",a.u2);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%x",a.u2);
            else if ( format == "Boolean" ) val = a.b1 ? "true" : "false";
            else if ( format == "Binary" ) val = binary(a,2);
            else val = "";
            break;
        case 4:
            if ( format == "Decimal" ) val.sprintf("%d",a.i4);
            else if ( format == "Unsigned decimal" ) val.sprintf("%d",a.u4);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%x",a.u4);
            else if ( format == "Floating point" ) val.sprintf("%g",a.f4);
            else if ( format == "Boolean" ) val = a.b1 ? "true" : "false";
            else if ( format == "Binary" ) val = binary(a,4);
            else if ( format == "Binary fp" ) val = binaryFloat(a);
            else if ( format == "Fields" ) val = fieldsFloat(a);
            else val = "";
            break;
        case 8:
#ifdef Q_WS_WIN
            if ( format == "Decimal" ) val.sprintf("%lld",a.i8);
            else if ( format == "Unsigned decimal" ) val.sprintf("%llu",a.u8);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%llx",a.u8);
#else
            if ( format == "Decimal" ) val.sprintf("%ld",a.i8);
            else if ( format == "Unsigned decimal" ) val.sprintf("%ld",a.u8);
            else if ( format == "Hexadecimal" ) val.sprintf("0x%lx",a.u8);
#endif
            else if ( format == "Floating point" ) val.sprintf("%g",a.f8);
            else if ( format == "Boolean" ) val = a.b1 ? "true" : "false";
            else if ( format == "Binary" ) val = binary(a,8);
            else if ( format == "Binary fp" ) val = binaryDouble(a);
            else if ( format == "Fields" ) val = fieldsDouble(a);
            else val = "";
            break;
        default:
            val = "";
        }
    } else if ( stringValue.indexOf("Illegal") >= 0 ) {
        val = stringValue;
    } else if ( classes.contains(type) ) {
        val = "";
    } else {
        val = stringValue;
    }
    //qDebug() << "value" << name << format << val;
    //qDebug() << "isSimple" << isSimple << ",  size" << size << ",  last" 
             //<< last;
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
        } else if ( t.indexOf("float") >= 0 || t.indexOf("double") >= 0 ||
                    t.indexOf("real") >= 0 ) {
            format = "Floating point";
        } else if ( t.indexOf("bool") >= 0 || t.indexOf("logical") >= 0 ) {
            format = "Boolean";
        } else {
            format = "Hexadecimal";
        }
    } else {
        isSimple = false;
        if ( t == "char **" ) format = "String array";
        else if ( t == "char *" ) format = "String";
        else if ( t.indexOf(" *") >= 0 ) format = "Pointer";
        else format = "";
    }

    setText(1,type);
    if ( isSimple && last == 0 ) {
        setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    } else {
        setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    }
}

void DataItem::setValue(QString v)
{
    bool ok;

    stringValue = v;
    if ( isSimple && last == 0 ) {
        a.u8 = 0;
        switch ( size ) {
        case 1:
            a.u1 = v.toUInt(&ok,16);
            break;
        case 2:
            a.u2 = v.toUInt(&ok,16);
            break;
        case 4:
            a.u4 = v.toUInt(&ok,16);
            break;
        case 8:
#ifdef Q_WS_WIN
            a.u8 = v.toULongLong(&ok,16);
#else
            a.u8 = v.toULong(&ok,16);
#endif
            break;
        }
        //qDebug() << name << size << v << a.u8 << value();
    }
    setText(2,value());
}

void DataItem::setRange(int f, int l)
{
    first = f;
    last = l;
    setName(name);
}

void DataTree::setBinary()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Binary";
    d->setText(2,d->value());
}

void DataTree::setBool()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Boolean";
    d->setText(2,d->value());
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

void DataTree::setFloatingPoint()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Floating point";
    d->setText(2,d->value());
}

void DataTree::setBinaryFP()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Binary fp";
    d->setText(2,d->value());
}

void DataTree::setFields()
{
    DataItem *d = (DataItem *)currentItem();
    d->format = "Fields";
    d->setText(2,d->value());
}

void DataTree::editUserVariable()
{
    DataItem *d = (DataItem *)currentItem();
    DefineVariableDialog *dialog = new DefineVariableDialog;
    dialog->nameEdit->setText(d->name);
    dialog->addressEdit->setText(d->address);
    dialog->formatCombo->setCurrentIndex(dialog->formatCombo->findText(d->format));
    dialog->typeCombo->setCurrentIndex(dialog->typeCombo->findText(QString("%1").arg(d->type)));
    dialog->firstEdit->setText(QString("%1").arg(d->first));
    dialog->lastEdit->setText(QString("%1").arg(d->last));
    if ( dialog->exec() ) {
        userDefinedMap->remove(d->name);
        d->setName(dialog->nameEdit->text());
        d->address = dialog->addressEdit->text();
        d->first = dialog->firstEdit->text().toInt();
        d->last = dialog->lastEdit->text().toInt();
        d->setType(dialog->typeCombo->currentText());
        d->format = dialog->formatCombo->currentText();
        userDefinedMap->insert(d->name,d);
        d->setText(2,d->value());
    }
    delete dialog;
}

void DataTree::deleteUserVariable()
{
    DataItem *item = (DataItem *)currentItem();
    QList<QTreeWidgetItem*> kids = item->takeChildren();
    foreach ( QTreeWidgetItem *t, kids ) {
        ((DataItem *)t)->removeSubTree();
        delete t;
    }
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
        QMenu menu(tr("Variable menu"));
        menu.addAction(tr("Edit variable"),this,SLOT(editUserVariable()));
        menu.addAction(tr("Delete variable"),this,SLOT(deleteUserVariable()));
        menu.exec(QCursor::pos());
    } else if ( item->isSimple ) {
        if ( type.indexOf("char") >= 0 ) {
            QMenu menu(tr("Character menu"));
            menu.addAction(tr("Character"),this,SLOT(setCharacter()));
            menu.addAction(tr("Decimal"),this,SLOT(setDecimal()));
            menu.addAction(tr("Unsigned decimal"),this,SLOT(setUnsignedDecimal()));
            menu.addAction(tr("Hexadecimal"),this,SLOT(setHexadecimal()));
            menu.addAction(tr("Binary"),this,SLOT(setBinary()));
            menu.exec(QCursor::pos());
        } else if ( type.indexOf("short") >= 0 || type.indexOf("int") >= 0 ||
                    type.indexOf("long") >= 0 ) {
            QMenu menu(tr("Integer menu"));
            menu.addAction(tr("Signed decimal"),this,SLOT(setDecimal()));
            menu.addAction(tr("Unsigned decimal"),this,SLOT(setUnsignedDecimal()));
            menu.addAction(tr("Hexadecimal"),this,SLOT(setHexadecimal()));
            menu.addAction(tr("Binary"),this,SLOT(setBinary()));
            menu.exec(QCursor::pos());
        } else if ( type == "float" || type == "double" ||
                    type.indexOf("real") >= 0 ) {
            QMenu menu(tr("Float menu"));
            menu.addAction(tr("Floating point"),this,SLOT(setFloatingPoint()));
            menu.addAction(tr("Hexadecimal"),this,SLOT(setHexadecimal()));
            menu.addAction(tr("Binary"),this,SLOT(setBinary()));
            menu.addAction(tr("Binary fp"),this,SLOT(setBinaryFP()));
            menu.addAction(tr("Fields"),this,SLOT(setFields()));
            menu.exec(QCursor::pos());
        } else if ( type == "bool" || type.indexOf("logical") >= 0 ) {
            QMenu menu("Boolean menu");
            menu.addAction(tr("Boolean"),this,SLOT(setBool()));
            menu.addAction(tr("Binary"),this,SLOT(setBinary()));
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
    DataItem *stack;

    ::globalMap = globalMap = new DataMap;
    ::localMap = localMap  = new DataMap;
    ::parameterMap = parameterMap = new DataMap;
    ::userDefinedMap = userDefinedMap = new DataMap;

    ::globals = globals = addDataItem(globalMap,tr("globals"),"","");
    ::locals = locals = addDataItem(localMap,tr("locals"),"","");
    ::parameters = parameters = addDataItem(parameterMap,tr("parameters"),"","");
    ::userDefined = userDefined = addDataItem(userDefinedMap,tr("user-defined"),"","");

    addTopLevelItem(globals);
    //globals->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    globals->setExpanded(true);
    addTopLevelItem(locals);
    //locals->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    locals->setExpanded(true);
    addTopLevelItem(parameters);
    //parameters->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    parameters->setExpanded(true);
    addTopLevelItem(userDefined);
    //userDefined->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    userDefined->setExpanded(true);
    stack = addDataItem(globalMap,"stack","unsigned long *","");
    stack->address = "$rsp";
    stack->first = 0;
    stack->last = 5;
    globals->addChild(stack);
    
    connect ( this, SIGNAL(itemExpanded(QTreeWidgetItem*)),
              this, SLOT(expandDataItem(QTreeWidgetItem*)) );
    connect ( this, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
              this, SLOT(collapseDataItem(QTreeWidgetItem*)) );
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

void DataTree::collapseDataItem(QTreeWidgetItem *item)
{
    int index;

    DataItem *it = (DataItem *)item;
    //qDebug() << "collapse";
    index = indexOfTopLevelItem(item);
    //qDebug() << "index" << index;
    if ( index >= 0 ) return;
    //qDebug() << "collapsing";
    it->setExpanded(false);
}

void DataTree::expandDataItem(QTreeWidgetItem *item)
{
    DataItem *it = (DataItem *)item;
    DataItem *d;
    QString fullName;
    ClassDefinition c;
    int n;

    //if ( item == globals ) qDebug() << "globals expand";
    c = classes[it->type];
    QList<QTreeWidgetItem*> kids = it->takeChildren();
    foreach ( QTreeWidgetItem *t, kids ) {
        //if ( item == globals ) qDebug() << "globals delete child";
        if ( item != globals ) {
            ((DataItem *)t)->removeSubTree();
            delete t;
        }
    }
    if ( c.name != "" ) {
        foreach ( VariableDefinition v, c.members ) {
            fullName = it->name + "." + v.name;
            //if ( item == globals ) qDebug() << "globals adding child" << fullName;
            d = addDataItem(it->map,fullName,v.type,"");
            if ( it->map == globalMap ) {
                d->address="&(::"+fullName+")";
            } else {
                d->address="&("+fullName+")";
            }
            it->addChild(d);
            dataWindow->request(d);
        }
    } else if ( it->userDefined ) {
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        if ( dialog->exec() ) {
            int min = dialog->min;
            int max = dialog->max;
            QString type = it->type;
            for ( int i = min; i <= max; i++ ) {
                fullName = it->name+QString("[%1]").arg(i);
                d = addDataItem(it->map,fullName,type,"");
                d->address=QString("%1+%2").arg(it->address).
                           arg(it->size*i);
                it->addChild(d);
                dataWindow->request(d);
            }
        }
        delete dialog;
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
                if ( it->address == "$rsp" ) {
                    d->address = QString("$rsp+%1").arg(i*8);
                    d->format = "Hexadecimal";
                } else {
                    d->address="&("+fullName+")";
                }
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
            dialog->setMin(0);
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
                    } else if ( newType == "float" || newType == "double" ||
                                newType.indexOf("real") > 0 ) {
                        format = "Floating point";
                    } else if ( newType == "bool" ||
                                newType.indexOf("logical") >= 0 ) {
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
    } else if ( it->isFortran && it->dimensions.size() > 0 ) {
        ArrayBoundsDialog *dialog = new ArrayBoundsDialog();
        dialog->setMin(it->dimensions[0].first);
        dialog->setMax(it->dimensions[0].last);
        if ( dialog->exec() ) {
            int min = dialog->min;
            int max = dialog->max;
            QString type = it->type;
            int n1 = type.lastIndexOf("(");
            int n2 = type.indexOf(",");
            if ( n2 < 0 ) {
                type = type.left(n1-1);
            } else {
                type = it->type.left(n1) + it->type.mid(n2+1);
            }
            int isize = 8;
            QString format;
            if ( simpleTypes.contains(type) ) {
                isize = sizeForType[type];
                if ( type == "char" ) {
                    format = "Character";
                } else if ( type == "float" || type == "double" ||
                            type.indexOf("real") > 0 ) {
                    format = "Floating point";
                } else if ( type == "bool" ||
                            type.indexOf("logical") >= 0 ) {
                    format = "Boolean";
                } else {
                    format = "Decimal";
                }
            }

            for ( int i = min; i <= max; i++ ) {
                fullName = it->name+QString("[%1]").arg(i);
                d = addDataItem(it->map,fullName,type,"");
                d->size = isize;
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

void DataWindow::receiveGlobals(QList<VariableDefinition> vars)
{
    DataItem *item;
    
    foreach ( VariableDefinition v, vars ) {
        item = globalMap->value(v.name);
        if ( item == 0 ) {
            item = dataTree->addDataItem(globalMap,v.name,v.type,v.value);
            item->address = QString("&(::%1").arg(v.name);
            foreach ( Limits d, v.dimensions ) {
                item->dimensions.append(d);
            }
            item->isFortran = v.isFortran;
            globals->addChild(item);
        } else {
            item->setValue(v.value);
            item->dimensions.clear();
            foreach ( Limits d, v.dimensions ) {
                item->dimensions.append(d);
            }
            item->isFortran = v.isFortran;
            int n = item->childCount();
            for ( int j = 0; j < n; j++ ) {
                request((DataItem *)item->child(j));
            }
        }
    }
    //qDebug() << "rg sort";
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

void DataWindow::receiveLocals(QList<VariableDefinition> vars)
{
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
    foreach ( VariableDefinition v, vars ) {
        item = localMap->value(v.name);
        if ( item == 0 ) {
            item = dataTree->addDataItem(localMap,v.name,v.type,v.value);
            item->address = QString("&(%1)").arg(v.name);
            foreach ( Limits d, v.dimensions ) {
                item->dimensions.append(d);
            }
            item->isFortran = v.isFortran;
            locals->addChild(item);
        } else {
            item->setValue(v.value);
            item->dimensions.clear();
            foreach ( Limits d, v.dimensions ) {
                item->dimensions.append(d);
            }
            item->isFortran = v.isFortran;
            int n = item->childCount();
            for ( int j = 0; j < n; j++ ) {
                request((DataItem *)item->child(j));
            }
        }
    }
    //qDebug() << "rl sort";
    locals->sortChildren(0,Qt::AscendingOrder);
}

void DataWindow::receiveParameters(QList<VariableDefinition> vars)
{
    DataItem *item;
    
    foreach ( VariableDefinition v, vars ) {
        //qDebug() << i << names[i] << types[i] << values[i];
        item = parameterMap->value(v.name);
        if ( item == 0 ) {
            item = dataTree->addDataItem(parameterMap,v.name,v.type,v.value);
            item->address = QString("&(%1)").arg(v.name);
            foreach ( Limits d, v.dimensions ) {
                item->dimensions.append(d);
            }
            item->isFortran = v.isFortran;
            parameters->addChild(item);
        } else {
            item->setValue(v.value);
            item->dimensions.clear();
            foreach ( Limits d, v.dimensions ) {
                item->dimensions.append(d);
            }
            item->isFortran = v.isFortran;
            int n = item->childCount();
            for ( int j = 0; j < n; j++ ) {
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
