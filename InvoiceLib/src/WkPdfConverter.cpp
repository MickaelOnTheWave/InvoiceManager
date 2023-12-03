#include "WkPdfConverter.h"

#include <QDir>
#include <QTextStream>

#include <pdfconverter.h>
#include <pdfsettings.hh>
#include <utilities.h>
#include "progressfeedback.h"

using namespace wkhtmltopdf::settings;
using namespace wkhtmltopdf;

WkPdfConverter::WkPdfConverter()
{
}

WkPdfConverter::~WkPdfConverter()
{
}

bool WkPdfConverter::ConvertHtmlToFile(const QString& content, const QString& filename)
{
   const bool ok = writeToTempFile(content);
   if (!ok)
      return false;

   PdfGlobal globalSettings;
   globalSettings.out = filename;
   PdfObject def;
   def.page = tempFile;

   bool use_graphics=true;
   MyLooksStyle * style = new MyLooksStyle();
   //a.setStyle(style);

   //Create the actual page converter to convert the pages
   PdfConverter converter(globalSettings);
   QObject::connect(&converter, SIGNAL(producingForms(bool)), style, SLOT(producingForms(bool)));
   QObject::connect(&converter, SIGNAL(checkboxSvgChanged(const QString &)), style, SLOT(setCheckboxSvg(const QString &)));
   QObject::connect(&converter, SIGNAL(checkboxCheckedSvgChanged(const QString &)), style, SLOT(setCheckboxCheckedSvg(const QString &)));
   QObject::connect(&converter, SIGNAL(radiobuttonSvgChanged(const QString &)), style, SLOT(setRadioButtonSvg(const QString &)));
   QObject::connect(&converter, SIGNAL(radiobuttonCheckedSvgChanged(const QString &)), style, SLOT(setRadioButtonCheckedSvg(const QString &)));

   ProgressFeedback feedback(globalSettings.logLevel, converter);
   converter.addResource(def);

   const bool success = converter.convert();
   const int errorCode = handleError(success, converter.httpErrorCode());
   removeTempFile();
   return errorCode == EXIT_SUCCESS;
}

bool WkPdfConverter::writeToTempFile(const QString& content)
{
   QFile f(getTempFilePath());
   if (!f.open(QIODevice::ReadWrite))
      return false;

   QTextStream stream(&f);
   stream << content << Qt::endl;
   return true;
}

bool WkPdfConverter::removeTempFile()
{
   QFile f(getTempFilePath());
   return f.remove();
}

QString WkPdfConverter::getTempFilePath() const
{
   return QDir::tempPath() + "/" + tempFile;
}
