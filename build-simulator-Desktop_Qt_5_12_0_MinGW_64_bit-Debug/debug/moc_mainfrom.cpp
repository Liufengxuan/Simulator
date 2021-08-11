/****************************************************************************
** Meta object code from reading C++ file 'mainfrom.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../simulator/mainfrom.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainfrom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainFrom_t {
    QByteArrayData data[13];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainFrom_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainFrom_t qt_meta_stringdata_MainFrom = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MainFrom"
QT_MOC_LITERAL(1, 9, 6), // "doWork"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 10), // "Sim_Handle"
QT_MOC_LITERAL(4, 28, 19), // "QMap<QString,float>"
QT_MOC_LITERAL(5, 48, 3), // "pos"
QT_MOC_LITERAL(6, 52, 16), // "const NCCommand*"
QT_MOC_LITERAL(7, 69, 2), // "nc"
QT_MOC_LITERAL(8, 72, 9), // "mAndTCode"
QT_MOC_LITERAL(9, 82, 17), // "btn_start_clicked"
QT_MOC_LITERAL(10, 100, 16), // "btn_stop_clicked"
QT_MOC_LITERAL(11, 117, 17), // "btn_reset_clicked"
QT_MOC_LITERAL(12, 135, 12) // "EditSimSpeed"

    },
    "MainFrom\0doWork\0\0Sim_Handle\0"
    "QMap<QString,float>\0pos\0const NCCommand*\0"
    "nc\0mAndTCode\0btn_start_clicked\0"
    "btn_stop_clicked\0btn_reset_clicked\0"
    "EditSimSpeed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainFrom[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    3,   47,    2, 0x08 /* Private */,
       9,    0,   54,    2, 0x08 /* Private */,
      10,    0,   55,    2, 0x08 /* Private */,
      11,    0,   56,    2, 0x08 /* Private */,
      12,    1,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6, QMetaType::QStringList,    5,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void MainFrom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainFrom *_t = static_cast<MainFrom *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->doWork((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->Sim_Handle((*reinterpret_cast< QMap<QString,float>(*)>(_a[1])),(*reinterpret_cast< const NCCommand*(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3]))); break;
        case 2: _t->btn_start_clicked(); break;
        case 3: _t->btn_stop_clicked(); break;
        case 4: _t->btn_reset_clicked(); break;
        case 5: _t->EditSimSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainFrom::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainFrom::doWork)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainFrom::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_MainFrom.data,
    qt_meta_data_MainFrom,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainFrom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainFrom::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainFrom.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainFrom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void MainFrom::doWork(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
