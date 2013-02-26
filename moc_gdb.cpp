/****************************************************************************
** Meta object code from reading C++ file 'gdb.h'
**
** Created: Wed Feb 13 13:20:43 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gdb.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gdb.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GDB[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: signature, parameters, type, tag, flags
       4,   33,   35,   35, 0x05,
      36,   35,   35,   35, 0x05,
      57,   35,   35,   35, 0x05,
      81,   33,   35,   35, 0x05,
     107,   35,   35,   35, 0x05,
     146,   35,   35,   35, 0x05,
     184,   35,   35,   35, 0x05,
     226,  270,   35,   35, 0x05,
     278,  312,   35,   35, 0x05,
     327,   35,   35,   35, 0x05,
     350,   35,   35,   35, 0x05,
     377,   35,   35,   35, 0x05,
     389,   35,   35,   35, 0x05,

 // slots: signature, parameters, type, tag, flags
     400,  461,   35,   35, 0x0a,
     499,   35,   35,   35, 0x0a,
     508,   35,   35,   35, 0x0a,
     517,   35,   35,   35, 0x0a,
     530,   35,   35,   35, 0x0a,
     539,  560,   35,   35, 0x0a,
     568,  633,   35,   35, 0x0a,
     678,   33,   35,   35, 0x0a,
     705,   33,   35,   35, 0x0a,
     735,   35,   35,   35, 0x0a,
     762,   35,   35,   35, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GDB[] = {
    "GDB\0nextInstruction(QString,int)\0,\0\0"
    "sendRegs(StringHash)\0sendFpRegs(QStringList)\0"
    "sendData(QString,QString)\0"
    "sendGlobals(QList<VariableDefinition>)\0"
    "sendLocals(QList<VariableDefinition>)\0"
    "sendParameters(QList<VariableDefinition>)\0"
    "sendClasses(QHash<QString,ClassDefinition>)\0"
    "classes\0sendVar(DataMap*,QString,QString)\0"
    "map,name,value\0dataReady(QStringList)\0"
    "sendBackTrace(QStringList)\0resetData()\0"
    "endFlash()\0"
    "doRun(QString,QString,QStringList,QList<IntSet>,QStringList)\0"
    "exe,options,files,breakpoints,globals\0"
    "doNext()\0doStep()\0doContinue()\0doStop()\0"
    "getData(QStringList)\0request\0"
    "requestVar(DataMap*,QString,QString,QString,QString,int,int,int)\0"
    "map,name,address,type,format,size,first,last\0"
    "setBreakpoint(QString,int)\0"
    "deleteBreakpoint(QString,int)\0"
    "receiveWorkingDir(QString)\0setEOF()\0"
};

void GDB::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GDB *_t = static_cast<GDB *>(_o);
        switch (_id) {
        case 0: _t->nextInstruction((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->sendRegs((*reinterpret_cast< StringHash(*)>(_a[1]))); break;
        case 2: _t->sendFpRegs((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 3: _t->sendData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->sendGlobals((*reinterpret_cast< QList<VariableDefinition>(*)>(_a[1]))); break;
        case 5: _t->sendLocals((*reinterpret_cast< QList<VariableDefinition>(*)>(_a[1]))); break;
        case 6: _t->sendParameters((*reinterpret_cast< QList<VariableDefinition>(*)>(_a[1]))); break;
        case 7: _t->sendClasses((*reinterpret_cast< QHash<QString,ClassDefinition>(*)>(_a[1]))); break;
        case 8: _t->sendVar((*reinterpret_cast< DataMap*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 9: _t->dataReady((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 10: _t->sendBackTrace((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 11: _t->resetData(); break;
        case 12: _t->endFlash(); break;
        case 13: _t->doRun((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< QList<IntSet>(*)>(_a[4])),(*reinterpret_cast< QStringList(*)>(_a[5]))); break;
        case 14: _t->doNext(); break;
        case 15: _t->doStep(); break;
        case 16: _t->doContinue(); break;
        case 17: _t->doStop(); break;
        case 18: _t->getData((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 19: _t->requestVar((*reinterpret_cast< DataMap*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 20: _t->setBreakpoint((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: _t->deleteBreakpoint((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->receiveWorkingDir((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->setEOF(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GDB::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GDB::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GDB,
      qt_meta_data_GDB, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GDB::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GDB::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GDB::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GDB))
        return static_cast<void*>(const_cast< GDB*>(this));
    return QObject::qt_metacast(_clname);
}

int GDB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void GDB::nextInstruction(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GDB::sendRegs(StringHash _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GDB::sendFpRegs(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GDB::sendData(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GDB::sendGlobals(QList<VariableDefinition> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GDB::sendLocals(QList<VariableDefinition> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void GDB::sendParameters(QList<VariableDefinition> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void GDB::sendClasses(QHash<QString,ClassDefinition> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void GDB::sendVar(DataMap * _t1, QString _t2, QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void GDB::dataReady(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void GDB::sendBackTrace(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void GDB::resetData()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void GDB::endFlash()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}
QT_END_MOC_NAMESPACE
