#ifndef WKPDFCONVERTER_H
#define WKPDFCONVERTER_H

#include <QString>

class WkPdfConverter
{
public:
   WkPdfConverter();
   virtual ~WkPdfConverter();

   bool ConvertHtmlToFile(const QString& content, const QString& filename);

private:
   bool writeToTempFile(const QString& content);
   bool removeTempFile();

   QString getTempFilePath() const;


   const QString tempFile = "tempFile.html";
};

#endif
