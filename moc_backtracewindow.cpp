/****************************************************************************
** Meta object code from reading C++ file 'backtracewindow.h'
**
** Created: Wed Feb 13 13:20:45 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "backtracewindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backtracewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BackTraceWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   46,   46,   46, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BackTraceWindow[] = {
    "BackTraceWindow\0receiveBackTrace(QStringList)\0"
    "\0"
};

void BackTraceWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BackTraceWindow *_t = static_cast<BackTraceWindow *>(_o);
        switch (_id) {
        case 0: _t->receiveBackTrace((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BackTraceWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BackTraceWindow::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_BackTraceWindow,
      qt_meta_data_BackTraceWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BackTraceWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BackTraceWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BackTraceWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BackTraceWindow))
        return static_cast<void*>(const_cast< BackTraceWindow*>(this));
    return QFrame::qt_metacast(_clname);
}

int BackTraceWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
