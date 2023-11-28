#include "PdfConverter.h"

#include "externals/wkhtmltopdf/src/lib/pdf.h"

WkPdfConverter::WkPdfConverter()
{
   const int successCode = 1;
   const int unused = 1;
   const int retValue = wkhtmltopdf_init(unused);
   initialized = (retValue == successCode);
}

WkPdfConverter::~WkPdfConverter()
{
   if (initialized)
      wkhtmltopdf_deinit();
}

bool WkPdfConverter::ConvertHtml(const QString& content)
{
   if (!initialized)
      return false;

   wkhtmltopdf_global_settings* pdfSettings = wkhtmltopdf_create_global_settings();
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
   wkhtmltopdf_destroy_global_settings(pdfSettings);
}

QByteArray WkPdfConverter::GetOutput() const
{

}
