/****************************************************************************
** Meta object code from reading C++ file 'sourceframe.h'
**
** Created: Wed Feb 13 13:20:33 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sourceframe.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sourceframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SourceFrame[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      46,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   73,  111,  111, 0x05,
     112,  111,  111,  111, 0x05,
     121,  111,  111,  111, 0x05,
     130,  111,  111,  111, 0x05,
     143,  111,  111,  111, 0x05,

 // slots: signature, parameters, type, tag, flags
     152,  181,  111,  111, 0x0a,
     191,  111,  111,  111, 0x0a,
     219,  235,  111,  111, 0x0a,
     241,  111,  111,  111, 0x0a,
     252,  111,  111,  111, 0x0a,
     259,  111,  111,  111, 0x0a,
     268,  111,  111,  111, 0x0a,
     276,  111,  111,  111, 0x0a,
     282,  111,  111,  111, 0x0a,
     289,  111,  111,  111, 0x0a,
     296,  111,  111,  111, 0x0a,
     307,  111,  111,  111, 0x0a,
     314,  111,  111,  111, 0x0a,
     320,  111,  111,  111, 0x0a,
     327,  111,  111,  111, 0x0a,
     335,  111,  111,  111, 0x0a,
     342,  111,  111,  111, 0x0a,
     349,  111,  111,  111, 0x0a,
     361,  111,  111,  111, 0x0a,
     374,  111,  111,  111, 0x0a,
     384,  111,  111,  111, 0x0a,
     396,  111,  111,  111, 0x0a,
     405,  111,  111,  111, 0x0a,
     416,  111,  111,  111, 0x0a,
     423,  111,  111,  111, 0x0a,
     437,  111,  111,  111, 0x0a,
     452,  111,  111,  111, 0x0a,
     461,  111,  111,  111, 0x0a,
     477,  111,  111,  111, 0x0a,
     492,  111,  111,  111, 0x0a,
     502,  111,  111,  111, 0x0a,
     515,  111,  111,  111, 0x0a,
     526,  111,  111,  111, 0x0a,
     537,  111,  111,  111, 0x0a,
     547,  111,  111,  111, 0x0a,
     559,  111,  111,  111, 0x0a,
     573,  111,  111,  111, 0x0a,
     592,  111,  111,  111, 0x0a,
     610,  111,  111,  111, 0x0a,
     633,  111,  111,  111, 0x0a,
     654,  674,  111,  111, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SourceFrame[] = {
    "SourceFrame\0"
    "doRun(QString,QString,QStringList,QList<IntSet>,QStringList)\0"
    "exe,options,files,breakpoints,globals\0"
    "\0doNext()\0doStep()\0doContinue()\0"
    "doStop()\0nextInstruction(QString,int)\0"
    "file,line\0setCommandLineVisible(bool)\0"
    "changedTab(int)\0index\0open(bool)\0"
    "save()\0saveAs()\0close()\0run()\0next()\0"
    "step()\0Continue()\0stop()\0cut()\0copy()\0"
    "paste()\0undo()\0redo()\0selectAll()\0"
    "selectNone()\0comment()\0unComment()\0"
    "indent()\0unIndent()\0find()\0pageForward()\0"
    "pageBackward()\0center()\0gotoFirstLine()\0"
    "gotoLastLine()\0gotoTop()\0gotoBottom()\0"
    "gotoLine()\0prettify()\0newFile()\0"
    "templateC()\0templateCpp()\0templateAssembly()\0"
    "templateFortran()\0updateCursorPosition()\0"
    "doTemplate(QAction*)\0insertFile(QString)\0"
    "f\0"
};

void SourceFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SourceFrame *_t = static_cast<SourceFrame *>(_o);
        switch (_id) {
        case 0: _t->doRun((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< QList<IntSet>(*)>(_a[4])),(*reinterpret_cast< QStringList(*)>(_a[5]))); break;
        case 1: _t->doNext(); break;
        case 2: _t->doStep(); break;
        case 3: _t->doContinue(); break;
        case 4: _t->doStop(); break;
        case 5: _t->nextInstruction((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->setCommandLineVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->changedTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->open((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->save(); break;
        case 10: _t->saveAs(); break;
        case 11: _t->close(); break;
        case 12: _t->run(); break;
        case 13: _t->next(); break;
        case 14: _t->step(); break;
        case 15: _t->Continue(); break;
        case 16: _t->stop(); break;
        case 17: _t->cut(); break;
        case 18: _t->copy(); break;
        case 19: _t->paste(); break;
        case 20: _t->undo(); break;
        case 21: _t->redo(); break;
        case 22: _t->selectAll(); break;
        case 23: _t->selectNone(); break;
        case 24: _t->comment(); break;
        case 25: _t->unComment(); break;
        case 26: _t->indent(); break;
        case 27: _t->unIndent(); break;
        case 28: _t->find(); break;
        case 29: _t->pageForward(); break;
        case 30: _t->pageBackward(); break;
        case 31: _t->center(); break;
        case 32: _t->gotoFirstLine(); break;
        case 33: _t->gotoLastLine(); break;
        case 34: _t->gotoTop(); break;
        case 35: _t->gotoBottom(); break;
        case 36: _t->gotoLine(); break;
        case 37: _t->prettify(); break;
        case 38: _t->newFile(); break;
        case 39: _t->templateC(); break;
        case 40: _t->templateCpp(); break;
        case 41: _t->templateAssembly(); break;
        case 42: _t->templateFortran(); break;
        case 43: _t->updateCursorPosition(); break;
        case 44: _t->doTemplate((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 45: _t->insertFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SourceFrame::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SourceFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_SourceFrame,
      qt_meta_data_SourceFrame, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SourceFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SourceFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SourceFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SourceFrame))
        return static_cast<void*>(const_cast< SourceFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int SourceFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 46)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 46;
    }
    return _id;
}

// SIGNAL 0
void SourceFrame::doRun(QString _t1, QString _t2, QStringList _t3, QList<IntSet> _t4, QStringList _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SourceFrame::doNext()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SourceFrame::doStep()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SourceFrame::doContinue()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void SourceFrame::doStop()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
