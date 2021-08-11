/****************************************************************************
** Meta object code from reading C++ file 'simworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../simulator/simworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Simulator__SimWorker_t {
    QByteArrayData data[10];
    char stringdata0[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Simulator__SimWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Simulator__SimWorker_t qt_meta_stringdata_Simulator__SimWorker = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Simulator::SimWorker"
QT_MOC_LITERAL(1, 21, 8), // "SimReady"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 19), // "QMap<QString,float>"
QT_MOC_LITERAL(4, 51, 3), // "pos"
QT_MOC_LITERAL(5, 55, 16), // "const NCCommand*"
QT_MOC_LITERAL(6, 72, 2), // "nc"
QT_MOC_LITERAL(7, 75, 13), // "realMAndTCode"
QT_MOC_LITERAL(8, 89, 6), // "DoWork"
QT_MOC_LITERAL(9, 96, 5) // "param"

    },
    "Simulator::SimWorker\0SimReady\0\0"
    "QMap<QString,float>\0pos\0const NCCommand*\0"
    "nc\0realMAndTCode\0DoWork\0param"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Simulator__SimWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, QMetaType::QStringList,    4,    6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    9,

       0        // eod
};

void Simulator::SimWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SimWorker *_t = static_cast<SimWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SimReady((*reinterpret_cast< QMap<QString,float>(*)>(_a[1])),(*reinterpret_cast< const NCCommand*(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3]))); break;
        case 1: _t->DoWork((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SimWorker::*)(QMap<QString,float> , const NCCommand * , QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimWorker::SimReady)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Simulator::SimWorker::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Simulator__SimWorker.data,
    qt_meta_data_Simulator__SimWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Simulator::SimWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Simulator::SimWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Simulator__SimWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Simulator::SimWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Simulator::SimWorker::SimReady(QMap<QString,float> _t1, const NCCommand * _t2, QStringList _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
