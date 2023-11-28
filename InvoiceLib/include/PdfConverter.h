#ifndef PDFCONVERTER_H
#define PDFCONVERTER_H

#include <QString>

class WkPdfConverter
{
public:
   WkPdfConverter();
   virtual ~WkPdfConverter();

   bool ConvertHtml(const QString& content);

   QByteArray GetOutput() const;

private:
   bool initialized;
};

#endif
