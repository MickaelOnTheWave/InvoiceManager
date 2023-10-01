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

#include "InvoicePrinter.h"

#include <iostream>
#include "Tools.h"

using std::cout;
using std::endl;
using std::string;

void InvoicePrinter::printSingle(const InvoiceUserData& data)
{
   const QString totalValue = QString::asprintf("%.2f", getTotalValue(data.details));

   cout << "|--------------------------------------------------------------------------|" << endl;
   cout << "|  Invoice Details                                                         |" << endl;
   cout << "|--------------------------------------------------------------------------|" << endl;
   cout << "  id            : " << data.id << endl;
   cout << "  Client        : " << data.clientCompany.name.toStdString() << endl;
   cout << "  Total Value   : " << totalValue.toStdString() << " " << data.currency.toStdString() << endl;
   cout << "  Date          : " << data.date.toString().toStdString() << endl;
   cout << "  Template      : " << data.templatePath.toStdString() << endl;
   cout << "  Stylesheet    : " << data.stylesheetPath.toStdString() << endl;
   cout << "|--------------------------------------------------------------------------|" << endl;
}

void InvoicePrinter::printMultiple(const std::vector<InvoiceUserData>& data)
{
   printInvoiceListHeader();
   for (const auto& invoiceData : data)
      printInvoiceOneLine(invoiceData);
   cout << "|--------------------------------------------------------------------------|" << endl;
}

double InvoicePrinter::getTotalValue(const std::vector<InvoiceDetail> details)
{
   auto operation = [] (double sum, const InvoiceDetail& detail) {
      return sum + detail.value;
   };
   return std::accumulate(details.begin(), details.end(), 0.0, operation);
}

void InvoicePrinter::printInvoiceListHeader()
{
   cout << "|--------------------------------------------------------------------------|" << endl;
   cout << "|  id  |                 Client             | Total value |      Date      |" << endl;
   cout << "|--------------------------------------------------------------------------|" << endl;
}

void InvoicePrinter::printInvoiceOneLine(const InvoiceUserData& data)
{
   string oneLinerStr;
   addLineInformation(oneLinerStr, data.id, 1, 7);
   addLineInformation(oneLinerStr, data.clientCompany.name, 9, 44);
   addLineInformation(oneLinerStr, getTotalValue(data.details), 46, 57);
   addLineInformation(oneLinerStr, data.date.toString("dd/MM/yyyy"), 59, 75);
   cout << oneLinerStr << endl;
}

void InvoicePrinter::addLineInformation(string& lineStr, const int data, const int minPosition, const int maxPosition)
{
   addLineInformation(lineStr, QString::number(data), minPosition, maxPosition);
}

void InvoicePrinter::addLineInformation(string& lineStr, const double data, const int minPosition, const int maxPosition)
{
   addLineInformation(lineStr, QString::asprintf("%.2f", data), minPosition, maxPosition);
}

void InvoicePrinter::addLineInformation(string& lineStr, const QString& data, const int minPosition, const int maxPosition)
{
   addLineInformation(lineStr, data.toStdString(), minPosition, maxPosition);
}

void InvoicePrinter::addLineInformation(string& lineStr, const string& data, const int minPosition, const int maxPosition)
{
   const int spacesToAdd = minPosition - lineStr.length();
   if (spacesToAdd > 0)
      lineStr += Tools::Spaces(spacesToAdd);

   const int maxDataLength = maxPosition - minPosition;
   const string dataToUse = (data.length() > maxDataLength) ? data.substr(0, maxDataLength) : data;
   lineStr += dataToUse;
}
