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

#ifndef INVOICEDATA_H
#define INVOICEDATA_H

#include <vector>
#include <QDate>

#include "CompanyData.h"
#include "InvoiceDetail.h"

class InvoiceBaseData
{
public:
   InvoiceBaseData() = default;
   InvoiceBaseData(const InvoiceBaseData& other);

   QString notes = "";
   QString currency = "€";
   QDate date;
   int id = -1;
};

class InvoiceDbData : public InvoiceBaseData
{
public:
    InvoiceDbData() = default;

    std::vector<int> detailsIds;
    int clientId = -1;
    int templateId = -1;
    int stylesheetId = -1;
};

// TODO there is a lot of data duplication with
// stylesheet/template datas.
// Maybe use a pointer to another collection?
class InvoiceUserData : public InvoiceBaseData
{
public:
   InvoiceUserData() = default;
   InvoiceUserData(const InvoiceBaseData& other);

   std::vector<InvoiceDetail> details;
   QString templateName;
   QString templateData;
   QString stylesheetName;
   QString stylesheetData;

   CompanyData userCompany;
   CompanyData clientCompany;
};

#endif // INVOICEDATA_H
