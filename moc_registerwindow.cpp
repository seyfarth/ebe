/****************************************************************************
** Meta object code from reading C++ file 'registerwindow.h'
**
** Created: Wed Feb 13 13:20:36 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "registerwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'registerwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RegisterWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   39,   39,   39, 0x0a,
      40,   39,   39,   39, 0x0a,
      53,   39,   39,   39, 0x0a,
      62,   39,   39,   39, 0x0a,
      78,   39,   39,   39, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RegisterWindow[] = {
    "RegisterWindow\0receiveRegs(StringHash)\0"
    "\0setDecimal()\0setHex()\0setDecimalAll()\0"
    "setHexAll()\0"
};

void RegisterWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RegisterWindow *_t = static_cast<RegisterWindow *>(_o);
        switch (_id) {
        case 0: _t->receiveRegs((*reinterpret_cast< StringHash(*)>(_a[1]))); break;
        case 1: _t->setDecimal(); break;
        case 2: _t->setHex(); break;
        case 3: _t->setDecimalAll(); break;
        case 4: _t->setHexAll(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RegisterWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RegisterWindow::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_RegisterWindow,
      qt_meta_data_RegisterWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RegisterWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RegisterWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RegisterWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RegisterWindow))
        return static_cast<void*>(const_cast< RegisterWindow*>(this));
    return QFrame::qt_metacast(_clname);
}

int RegisterWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
