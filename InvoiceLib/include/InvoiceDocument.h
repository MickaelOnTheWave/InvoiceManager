#ifndef INVOICEDOCUMENT_H
#define INVOICEDOCUMENT_H

#include "InvoiceData.h"

class InvoiceDocument
{
public:
   InvoiceDocument() = default;
   virtual ~InvoiceDocument() = default;

   void setData(const InvoiceUserData& data);

   QString CreateHtmlContent();

   /**
    * @brief CreateFile Creates a PDF file based on the invoice data
    * @param data
    * @return the path of the created file, or empty string in case of error.
    */
   void CreatePdfFile(const QString& file);

   int GetInvoiceId() const;

private:
   static QString readFileContent(const QString& filename);
   static QString fillTemplate(const QString& templateModel,
                               const InvoiceUserData& data);

   static QString buildReplaceAddress(const QString& recordedAddress);
   static QString buildReplaceDetails(const std::vector<InvoiceDetail>& details);
   static QString buildInvoiceTotal(const std::vector<InvoiceDetail>& details);

   InvoiceUserData invoiceData;
};

#endif
