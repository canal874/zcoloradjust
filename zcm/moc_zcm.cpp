/****************************************************************************
** ZColorMatching meta object code from reading C++ file 'zcm.h'
**
** Created: Mon Feb 25 01:08:54 2008
**      by: The Qt MOC ($Id: qt/src/moc/moc.y   2.3.3   edited 2001-10-17 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "zcm.h"
#include <qmetaobject.h>
#include <qapplication.h>



const char *ZColorMatching::className() const
{
    return "ZColorMatching";
}

QMetaObject *ZColorMatching::metaObj = 0;

void ZColorMatching::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QMainWindow::className(), "QMainWindow") != 0 )
	badSuperclassWarning("ZColorMatching","QMainWindow");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString ZColorMatching::tr(const char* s)
{
    return qApp->translate( "ZColorMatching", s, 0 );
}

QString ZColorMatching::tr(const char* s, const char * c)
{
    return qApp->translate( "ZColorMatching", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* ZColorMatching::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QMainWindow::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"ZColorMatching", "QMainWindow",
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}


const char *Canvas::className() const
{
    return "Canvas";
}

QMetaObject *Canvas::metaObj = 0;

void Canvas::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("Canvas","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString Canvas::tr(const char* s)
{
    return qApp->translate( "Canvas", s, 0 );
}

QString Canvas::tr(const char* s, const char * c)
{
    return qApp->translate( "Canvas", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* Canvas::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"Canvas", "QWidget",
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}


const char *MyToolBar::className() const
{
    return "MyToolBar";
}

QMetaObject *MyToolBar::metaObj = 0;

void MyToolBar::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("MyToolBar","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString MyToolBar::tr(const char* s)
{
    return qApp->translate( "MyToolBar", s, 0 );
}

QString MyToolBar::tr(const char* s, const char * c)
{
    return qApp->translate( "MyToolBar", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* MyToolBar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"MyToolBar", "QDialog",
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
