/****************************************************************************
** Meta object code from reading C++ file 'sourcewindow.h'
**
** Created: Wed Feb 13 13:20:31 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sourcewindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sourcewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FindReplaceDialog[] = {

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
      18,   25,   25,   25, 0x0a,
      26,   25,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FindReplaceDialog[] = {
    "FindReplaceDialog\0find()\0\0replace()\0"
};

void FindReplaceDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FindReplaceDialog *_t = static_cast<FindReplaceDialog *>(_o);
        switch (_id) {
        case 0: _t->find(); break;
        case 1: _t->replace(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData FindReplaceDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FindReplaceDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FindReplaceDialog,
      qt_meta_data_FindReplaceDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FindReplaceDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FindReplaceDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FindReplaceDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FindReplaceDialog))
        return static_cast<void*>(const_cast< FindReplaceDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FindReplaceDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
static const uint qt_meta_data_LineNumberDialog[] = {

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
      17,   27,   27,   27, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LineNumberDialog[] = {
    "LineNumberDialog\0setLine()\0\0"
};

void LineNumberDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LineNumberDialog *_t = static_cast<LineNumberDialog *>(_o);
        switch (_id) {
        case 0: _t->setLine(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LineNumberDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LineNumberDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LineNumberDialog,
      qt_meta_data_LineNumberDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LineNumberDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LineNumberDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LineNumberDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LineNumberDialog))
        return static_cast<void*>(const_cast< LineNumberDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int LineNumberDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_SourceEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   26,   33,   33, 0x05,
      34,   33,   33,   33, 0x05,

 // slots: signature, parameters, type, tag, flags
      70,   33,   33,   33, 0x08,
      84,   33,   33,   33, 0x08,
     101,  127,   33,   33, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SourceEdit[] = {
    "SourceEdit\0newHeight(int)\0height\0\0"
    "sendVariableDefinition(QStringList)\0"
    "printScroll()\0defineVariable()\0"
    "insertCompletion(QString)\0completion\0"
};

void SourceEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SourceEdit *_t = static_cast<SourceEdit *>(_o);
        switch (_id) {
        case 0: _t->newHeight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sendVariableDefinition((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 2: _t->printScroll(); break;
        case 3: _t->defineVariable(); break;
        case 4: _t->insertCompletion((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SourceEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SourceEdit::staticMetaObject = {
    { &QPlainTextEdit::staticMetaObject, qt_meta_stringdata_SourceEdit,
      qt_meta_data_SourceEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SourceEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SourceEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SourceEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SourceEdit))
        return static_cast<void*>(const_cast< SourceEdit*>(this));
    return QPlainTextEdit::qt_metacast(_clname);
}

int SourceEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SourceEdit::newHeight(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SourceEdit::sendVariableDefinition(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_LineNumberEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   43,   45,   45, 0x05,
      46,   43,   45,   45, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   45,   45,   45, 0x08,
      92,   45,   45,   45, 0x08,
     109,   45,   45,   45, 0x08,
     130,   45,   45,   45, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LineNumberEdit[] = {
    "LineNumberEdit\0sendBreakpoint(QString,int)\0"
    ",\0\0deleteBreakpoint(QString,int)\0"
    "setBreakpoint()\0dropBreakpoint()\0"
    "dropAllBreakpoints()\0ignore()\0"
};

void LineNumberEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LineNumberEdit *_t = static_cast<LineNumberEdit *>(_o);
        switch (_id) {
        case 0: _t->sendBreakpoint((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->deleteBreakpoint((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->setBreakpoint(); break;
        case 3: _t->dropBreakpoint(); break;
        case 4: _t->dropAllBreakpoints(); break;
        case 5: _t->ignore(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LineNumberEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LineNumberEdit::staticMetaObject = {
    { &QPlainTextEdit::staticMetaObject, qt_meta_stringdata_LineNumberEdit,
      qt_meta_data_LineNumberEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LineNumberEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LineNumberEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LineNumberEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LineNumberEdit))
        return static_cast<void*>(const_cast< LineNumberEdit*>(this));
    return QPlainTextEdit::qt_metacast(_clname);
}

int LineNumberEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void LineNumberEdit::sendBreakpoint(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LineNumberEdit::deleteBreakpoint(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_SourceWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   20,   20,   20, 0x0a,
      21,   20,   20,   20, 0x0a,
      28,   20,   20,   20, 0x0a,
      37,   20,   20,   20, 0x0a,
      51,   66,   20,   20, 0x0a,
      81,  103,   20,   20, 0x0a,
     109,  140,   20,   20, 0x0a,
     148,  103,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SourceWindow[] = {
    "SourceWindow\0open()\0\0save()\0saveAs()\0"
    "textChanged()\0newHeight(int)\0"
    "heightInPixels\0scrollBarChanged(int)\0"
    "value\0scrollBarRangeChanged(int,int)\0"
    "min,max\0sliderChanged(int)\0"
};

void SourceWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SourceWindow *_t = static_cast<SourceWindow *>(_o);
        switch (_id) {
        case 0: _t->open(); break;
        case 1: _t->save(); break;
        case 2: _t->saveAs(); break;
        case 3: _t->textChanged(); break;
        case 4: _t->newHeight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->scrollBarChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->scrollBarRangeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->sliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SourceWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SourceWindow::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_SourceWindow,
      qt_meta_data_SourceWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SourceWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SourceWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SourceWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SourceWindow))
        return static_cast<void*>(const_cast< SourceWindow*>(this));
    return QFrame::qt_metacast(_clname);
}

int SourceWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
