/****************************************************************************
** Meta object code from reading C++ file 'variable.h'
**
** Created: Wed Feb 13 13:20:44 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "variable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'variable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DefineVariableDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   38,   38,   38, 0x0a,
      39,   38,   38,   38, 0x0a,
      57,   38,   38,   38, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DefineVariableDialog[] = {
    "DefineVariableDialog\0defineVariable()\0"
    "\0checkChanged(int)\0typeChanged(QString)\0"
};

void DefineVariableDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DefineVariableDialog *_t = static_cast<DefineVariableDialog *>(_o);
        switch (_id) {
        case 0: _t->defineVariable(); break;
        case 1: _t->checkChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->typeChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DefineVariableDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DefineVariableDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DefineVariableDialog,
      qt_meta_data_DefineVariableDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DefineVariableDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DefineVariableDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DefineVariableDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DefineVariableDialog))
        return static_cast<void*>(const_cast< DefineVariableDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int DefineVariableDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_ArrayBoundsDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   35,   35,   35, 0x0a,
      36,   35,   35,   35, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ArrayBoundsDialog[] = {
    "ArrayBoundsDialog\0setArrayBounds()\0\0"
    "checkChanged(int)\0"
};

void ArrayBoundsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ArrayBoundsDialog *_t = static_cast<ArrayBoundsDialog *>(_o);
        switch (_id) {
        case 0: _t->setArrayBounds(); break;
        case 1: _t->checkChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ArrayBoundsDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ArrayBoundsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ArrayBoundsDialog,
      qt_meta_data_ArrayBoundsDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ArrayBoundsDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ArrayBoundsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ArrayBoundsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ArrayBoundsDialog))
        return static_cast<void*>(const_cast< ArrayBoundsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ArrayBoundsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
