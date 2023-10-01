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

#ifndef INVOICEPRINTER_H
#define INVOICEPRINTER_H

#include "InvoiceData.h"

class InvoicePrinter
{
public:
   static void printSingle(const InvoiceUserData& data);
   static void printMultiple(const std::vector<InvoiceUserData>& data);

private:
   static double getTotalValue(const std::vector<InvoiceDetail> details);

   static void printInvoiceListHeader();
   static void printInvoiceOneLine(const InvoiceUserData& data);

   static void addLineInformation(std::string& lineStr, const int data,
                                  const int minPosition, const int maxPosition);
   static void addLineInformation(std::string& lineStr, const double data,
                                  const int minPosition, const int maxPosition);
   static void addLineInformation(std::string& lineStr, const QString& data,
                                  const int minPosition, const int maxPosition);
   static void addLineInformation(std::string& lineStr, const std::string& data,
                                  const int minPosition, const int maxPosition);
};

#endif
