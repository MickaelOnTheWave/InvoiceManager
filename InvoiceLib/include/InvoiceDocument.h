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
