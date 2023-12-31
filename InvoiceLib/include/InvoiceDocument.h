/*
 * InvoiceManager
 * Copyright (C) 2023 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INVOICEDOCUMENT_H
#define INVOICEDOCUMENT_H

#include "InvoiceData.h"

class InvoiceDocument
{
public:
   InvoiceDocument() = default;
   virtual ~InvoiceDocument() = default;

   void setData(const InvoiceUserData& data);

   QString CreateHtmlContent() const;

   QString CreateStyledHtmlContent() const;

   /**
    * @brief CreateHtmlFile Creates an HTML file based on the invoice data
    * @param file the path of the file to create
    */
   bool CreateHtmlFile(const QString& file);

   /**
    * @brief CreatePdfFile Creates a PDF file based on the invoice data
    * @param file the path of the file to create
    */
   bool CreatePdfFile(const QString& file);

   /**
    * @brief CreateFile Creates a PDF file based on the invoice data
    * @param file The name pattern of the file to create
    */
   bool CreatePdfFileFromPattern(const QString& filenamePattern);

   int GetInvoiceId() const;

private:
   static QString fillTemplate(const QString& templateModel,
                               const InvoiceUserData& data);
   static QString addInternalCss(const QString& htmlContent,
                                 const QString& cssContent);

   static QString buildReplaceAddress(const QString& recordedAddress);
   static QString buildReplaceDetailsV8(const std::vector<InvoiceDetail>& details);
   static void replaceNewerDetails(QString& templateData, const std::vector<InvoiceDetail>& details);
   static QString buildInvoiceTotal(const std::vector<InvoiceDetail>& details);

   QString GetFileFromPattern(const QString& pattern) const;

   InvoiceUserData invoiceData;
};

#endif
