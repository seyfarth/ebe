/****************************************************************************
** Meta object code from reading C++ file 'datawindow.h'
**
** Created: Wed Feb 13 13:20:38 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "datawindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datawindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataTree[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,   42,   42,   42, 0x0a,
      43,   42,   42,   42, 0x0a,
      62,   42,   42,   42, 0x0a,
      83,   42,   42,   42, 0x0a,
      96,   42,   42,   42, 0x0a,
     115,   42,   42,   42, 0x0a,
     125,   42,   42,   42, 0x0a,
     137,   42,   42,   42, 0x0a,
     151,   42,   42,   42, 0x0a,
     163,   42,   42,   42, 0x0a,
     178,   42,   42,   42, 0x0a,
     199,   42,   42,   42, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataTree[] = {
    "DataTree\0expandDataItem(QTreeWidgetItem*)\0"
    "\0editUserVariable()\0deleteUserVariable()\0"
    "setDecimal()\0setFloatingPoint()\0"
    "setBool()\0setBinary()\0setBinaryFP()\0"
    "setFields()\0setCharacter()\0"
    "setUnsignedDecimal()\0setHexadecimal()\0"
};

void DataTree::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DataTree *_t = static_cast<DataTree *>(_o);
        switch (_id) {
        case 0: _t->expandDataItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->editUserVariable(); break;
        case 2: _t->deleteUserVariable(); break;
        case 3: _t->setDecimal(); break;
        case 4: _t->setFloatingPoint(); break;
        case 5: _t->setBool(); break;
        case 6: _t->setBinary(); break;
        case 7: _t->setBinaryFP(); break;
        case 8: _t->setFields(); break;
        case 9: _t->setCharacter(); break;
        case 10: _t->setUnsignedDecimal(); break;
        case 11: _t->setHexadecimal(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DataTree::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DataTree::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_DataTree,
      qt_meta_data_DataTree, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataTree::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataTree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataTree::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataTree))
        return static_cast<void*>(const_cast< DataTree*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int DataTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
static const uint qt_meta_data_DataWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   76,   84,   84, 0x05,

 // slots: signature, parameters, type, tag, flags
      85,   84,   84,   84, 0x08,
     124,   84,   84,   84, 0x08,
     136,  183,   84,   84, 0x08,
     185,  222,   84,   84, 0x08,
     239,   84,   84,   84, 0x08,
     281,   84,   84,   84, 0x08,
     322,   84,   84,   84, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DataWindow[] = {
    "DataWindow\0"
    "requestVar(DataMap*,QString,QString,QString,QString,int,int,int)\0"
    ",,,,,,,\0\0receiveVariableDefinition(QStringList)\0"
    "resetData()\0receiveClasses(QHash<QString,ClassDefinition>)\0"
    "c\0receiveVar(DataMap*,QString,QString)\0"
    "group,name,value\0"
    "receiveGlobals(QList<VariableDefinition>)\0"
    "receiveLocals(QList<VariableDefinition>)\0"
    "receiveParameters(QList<VariableDefinition>)\0"
};

void DataWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DataWindow *_t = static_cast<DataWindow *>(_o);
        switch (_id) {
        case 0: _t->requestVar((*reinterpret_cast< DataMap*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 1: _t->receiveVariableDefinition((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 2: _t->resetData(); break;
        case 3: _t->receiveClasses((*reinterpret_cast< QHash<QString,ClassDefinition>(*)>(_a[1]))); break;
        case 4: _t->receiveVar((*reinterpret_cast< DataMap*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->receiveGlobals((*reinterpret_cast< QList<VariableDefinition>(*)>(_a[1]))); break;
        case 6: _t->receiveLocals((*reinterpret_cast< QList<VariableDefinition>(*)>(_a[1]))); break;
        case 7: _t->receiveParameters((*reinterpret_cast< QList<VariableDefinition>(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DataWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DataWindow::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_DataWindow,
      qt_meta_data_DataWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataWindow))
        return static_cast<void*>(const_cast< DataWindow*>(this));
    return QFrame::qt_metacast(_clname);
}

int DataWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void DataWindow::requestVar(DataMap * _t1, QString _t2, QString _t3, QString _t4, QString _t5, int _t6, int _t7, int _t8)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
