
#include "progressfeedback.hh"
#include <QCommonStyle>
#include <QPainter>
#include <QStyleOption>
#include <QWebFrame>
#include <QtPlugin>
#include <cstdlib>
#include <iostream>
#include <qapplication.h>
#include <qglobal.h>
#include <string.h>
#include <pdfconverter.hh>
#include <pdfsettings.hh>
#include <utilities.hh>

#if defined(Q_OS_UNIX)
#include <locale.h>
#endif

using namespace wkhtmltopdf::settings;
using namespace wkhtmltopdf;

int main(int argc, char * argv[]) {

	PdfGlobal globalSettings;

   globalSettings.out = "hardcoded.pdf";
   PdfObject def;
   def.page = "https://www.free.fr";

	bool use_graphics=true;
	QApplication a(argc, argv, use_graphics);
	MyLooksStyle * style = new MyLooksStyle();
	a.setStyle(style);

	//Create the actual page converter to convert the pages
	PdfConverter converter(globalSettings);
	QObject::connect(&converter, SIGNAL(producingForms(bool)), style, SLOT(producingForms(bool)));
	QObject::connect(&converter, SIGNAL(checkboxSvgChanged(const QString &)), style, SLOT(setCheckboxSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(checkboxCheckedSvgChanged(const QString &)), style, SLOT(setCheckboxCheckedSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(radiobuttonSvgChanged(const QString &)), style, SLOT(setRadioButtonSvg(const QString &)));
	QObject::connect(&converter, SIGNAL(radiobuttonCheckedSvgChanged(const QString &)), style, SLOT(setRadioButtonCheckedSvg(const QString &)));

   ProgressFeedback feedback(globalSettings.logLevel, converter);
   converter.addResource(def);

	bool success = converter.convert();
	return handleError(success, converter.httpErrorCode());
}
