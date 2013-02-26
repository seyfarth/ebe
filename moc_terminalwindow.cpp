/****************************************************************************
** Meta object code from reading C++ file 'terminalwindow.h'
**
** Created: Wed Feb 13 13:20:34 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "terminalwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'terminalwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TerminalEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_TerminalEdit[] = {
    "TerminalEdit\0"
};

void TerminalEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TerminalEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TerminalEdit::staticMetaObject = {
    { &QPlainTextEdit::staticMetaObject, qt_meta_stringdata_TerminalEdit,
      qt_meta_data_TerminalEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TerminalEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TerminalEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TerminalEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TerminalEdit))
        return static_cast<void*>(const_cast< TerminalEdit*>(this));
    return QPlainTextEdit::qt_metacast(_clname);
}

int TerminalEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_InputEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      32,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_InputEdit[] = {
    "InputEdit\0sendEOF()\0\0endFlash()\0"
    "updateFlash()\0"
};

void InputEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        InputEdit *_t = static_cast<InputEdit *>(_o);
        switch (_id) {
        case 0: _t->sendEOF(); break;
        case 1: _t->endFlash(); break;
        case 2: _t->updateFlash(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData InputEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject InputEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_InputEdit,
      qt_meta_data_InputEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &InputEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *InputEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *InputEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InputEdit))
        return static_cast<void*>(const_cast< InputEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int InputEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void InputEdit::sendEOF()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_TerminalWindow[] = {

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
      15,   34,   39,   39, 0x08,
      40,   39,   39,   39, 0x08,
      56,   39,   39,   39, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TerminalWindow[] = {
    "TerminalWindow\0dataReady(QString)\0"
    "data\0\0lineEditReady()\0clearTerminalWindow()\0"
};

void TerminalWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TerminalWindow *_t = static_cast<TerminalWindow *>(_o);
        switch (_id) {
        case 0: _t->dataReady((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->lineEditReady(); break;
        case 2: _t->clearTerminalWindow(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TerminalWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TerminalWindow::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_TerminalWindow,
      qt_meta_data_TerminalWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TerminalWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TerminalWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TerminalWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TerminalWindow))
        return static_cast<void*>(const_cast< TerminalWindow*>(this));
    return QFrame::qt_metacast(_clname);
}

int TerminalWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
