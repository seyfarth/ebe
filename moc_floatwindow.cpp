/****************************************************************************
** Meta object code from reading C++ file 'floatwindow.h'
**
** Created: Wed Feb 13 13:20:39 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "floatwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'floatwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FloatWindow[] = {

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
      12,   39,   39,   39, 0x0a,
      40,   65,   39,   39, 0x0a,
      72,   65,   39,   39, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FloatWindow[] = {
    "FloatWindow\0receiveFpRegs(QStringList)\0"
    "\0formatRegister(QAction*)\0action\0"
    "formatAllRegisters(QAction*)\0"
};

void FloatWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FloatWindow *_t = static_cast<FloatWindow *>(_o);
        switch (_id) {
        case 0: _t->receiveFpRegs((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 1: _t->formatRegister((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 2: _t->formatAllRegisters((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FloatWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FloatWindow::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_FloatWindow,
      qt_meta_data_FloatWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FloatWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FloatWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FloatWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FloatWindow))
        return static_cast<void*>(const_cast< FloatWindow*>(this));
    return QFrame::qt_metacast(_clname);
}

int FloatWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
