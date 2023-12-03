#ifndef WKPDFCONVERTER_H
#define WKPDFCONVERTER_H

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
