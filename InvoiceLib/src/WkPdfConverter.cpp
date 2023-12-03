#include "WkPdfConverter.h"

#include <pdfconverter.h>
#include <pdfsettings.hh>
#include <utilities.h>
#include "progressfeedback.h"

using namespace wkhtmltopdf::settings;
using namespace wkhtmltopdf;

WkPdfConverter::WkPdfConverter()
{
   const int successCode = 1;
   const int unused = 1;
//   const int retValue = wkhtmltopdf_init(unused);
//   initialized = (retValue == successCode);
}

WkPdfConverter::~WkPdfConverter()
{
//   if (initialized)
//      wkhtmltopdf_deinit();
}

bool WkPdfConverter::ConvertHtml(const QString& content)
{
   if (!initialized)
      return false;

/*   wkhtmltopdf_global_settings* pdfSettings = wkhtmltopdf_create_global_settings();
   wkhtmltopdf_converter* pdfConverter =  wkhtmltopdf_create_converter(pdfSettings);

   wkhtmltopdf_object_settings* docSettings = wkhtmltopdf_create_object_settings();
   //int result = wkhtmltopdf_set_object_setting(docSettings, name, value);
   wkhtmltopdf_add_object(pdfConverter, docSettings, content.toUtf8().constData());

   QByteArray data;
   auto finishCallback = [](wkhtmltopdf_converter* converter, const int val) {

   };
   wkhtmltopdf_set_finished_callback(pdfConverter, finishCallback);

   wkhtmltopdf_convert(pdfConverter);

   wkhtmltopdf_destroy_object_settings(docSettings);
   wkhtmltopdf_destroy_converter(pdfConverter);
   wkhtmltopdf_destroy_global_settings(pdfSettings);*/



   PdfGlobal globalSettings;
   globalSettings.out = "hardcoded.pdf";
   PdfObject def;
   def.page = "https://www.free.fr";

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

   bool success = converter.convert();
   return handleError(success, converter.httpErrorCode());

}

QByteArray WkPdfConverter::GetOutput() const
{

}
