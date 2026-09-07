/****************************************************************************
** Meta object code from reading C++ file 'gformateurcours.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gformateurcours.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gformateurcours.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN15GFormateurCoursE_t {};
} // unnamed namespace

template <> constexpr inline auto GFormateurCours::qt_create_metaobjectdata<qt_meta_tag_ZN15GFormateurCoursE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GFormateurCours",
        "onAjouterFormateur",
        "",
        "onModifierFormateur",
        "onSupprimerFormateur",
        "onViderFormateur",
        "onAjouterCours",
        "onModifierCours",
        "onSupprimerCours",
        "onViderCours",
        "onRechercherFormateurs",
        "onRechercherCours",
        "onActualiserStatsFormateurs",
        "onActualiserStatsCours",
        "onGenererPdfFormateur",
        "onGenererDocCours",
        "onFormateurSelected",
        "QModelIndex",
        "index",
        "onCoursSelected"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onAjouterFormateur'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onModifierFormateur'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSupprimerFormateur'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onViderFormateur'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAjouterCours'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onModifierCours'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSupprimerCours'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onViderCours'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRechercherFormateurs'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRechercherCours'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActualiserStatsFormateurs'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onActualiserStatsCours'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onGenererPdfFormateur'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onGenererDocCours'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFormateurSelected'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 17, 18 },
        }}),
        // Slot 'onCoursSelected'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 17, 18 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GFormateurCours, qt_meta_tag_ZN15GFormateurCoursE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GFormateurCours::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15GFormateurCoursE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15GFormateurCoursE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15GFormateurCoursE_t>.metaTypes,
    nullptr
} };

void GFormateurCours::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GFormateurCours *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onAjouterFormateur(); break;
        case 1: _t->onModifierFormateur(); break;
        case 2: _t->onSupprimerFormateur(); break;
        case 3: _t->onViderFormateur(); break;
        case 4: _t->onAjouterCours(); break;
        case 5: _t->onModifierCours(); break;
        case 6: _t->onSupprimerCours(); break;
        case 7: _t->onViderCours(); break;
        case 8: _t->onRechercherFormateurs(); break;
        case 9: _t->onRechercherCours(); break;
        case 10: _t->onActualiserStatsFormateurs(); break;
        case 11: _t->onActualiserStatsCours(); break;
        case 12: _t->onGenererPdfFormateur(); break;
        case 13: _t->onGenererDocCours(); break;
        case 14: _t->onFormateurSelected((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 15: _t->onCoursSelected((*reinterpret_cast<std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *GFormateurCours::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GFormateurCours::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15GFormateurCoursE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int GFormateurCours::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
