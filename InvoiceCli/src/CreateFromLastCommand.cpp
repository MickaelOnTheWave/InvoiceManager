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

#include "CreateFromLastCommand.h"

#include <iostream>
#include "CliParametersDefinitions.h"
#include "InvoicePrinter.h"

using std::cout;
using std::endl;
using std::string;

void CreateFromLastCommand::Run(InvoiceDbController& controller, const CommandLineManager& cli)
{
   const QDate selectedDate = GetDateFromCli(cli.GetParameterValue(dateParam));
   if (selectedDate.isValid())
      RunCreateFromLastCommand(controller, selectedDate);
   else
      cout << "Error : invalid date specified." << endl;
}

void CreateFromLastCommand::RunCreateFromLastCommand(InvoiceDbController& controller, const QDate& date)
{
   InvoiceDbData dbData = controller.getInvoiceDbData(controller.getLastInvoiceId());

   InvoiceUserData userData = controller.toUserData(dbData);

   cout << "Here are is the invoice that will be created :" << endl;
   ++dbData.id; ++userData.id;
   dbData.date = userData.date = date;
   InvoicePrinter::printSingle(userData);

   const bool ok = controller.writeInvoice(dbData);
   if (!ok)
      cout << "Error creating new invoice." << endl;
}

QDate CreateFromLastCommand::GetDateFromCli(const string& dateValue)
{
   if (dateValue == "today")
      return QDate::currentDate();
   else if (dateValue == "lastDayOfThisMonth")
      return GetLastDayOfThisMonth();
   else if (dateValue.empty())
   {
      cout << "No date provided. Using last day of this month." << endl;
      return GetLastDayOfThisMonth();
   }

   return QDate::fromString(QString::fromStdString(dateValue), "DD/MM/yyyy");
}

QDate CreateFromLastCommand::GetLastDayOfThisMonth()
{
   QDate today = QDate::currentDate();
   return QDate(today.year(), today.month(), today.daysInMonth());
}
