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
   std::vector<InvoiceDetail> details;
   QString notes = "";
   QString currency;
};

class InvoiceData : public InvoiceBaseData
{
public:
    InvoiceData();

    int clientId = -1;
    int templateId = -1;
    int stylesheetId = -1;
};

class InvoiceTemplateData : public InvoiceBaseData
{
public:
   QString templatePath;
   QString stylesheetPath;

   CompanyData userCompany;
   CompanyData clientCompany;
   QDate date;
   int id;
};

#endif // INVOICEDATA_H
