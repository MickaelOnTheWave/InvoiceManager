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

#include <algorithm>
#include <iostream>
#include <string>

#include "commandlinemanager.h"
#include "InvoiceDbController.h"
#include "InvoicePrinter.h"

// TODO Make ToolsLib usable lib and use it here instead of using copied cpp/h
// TODO use same version in installer, cli, gui and everywhere
// TODO Next :
// - Implement Generate PDF command
// - Implement file template command
// TODO Refactor all this, there should be some classes for command / printing etc

using std::string;
using std::cout;
using std::endl;

const string dbCommand = "db";
const string listCommand = "list";
const string showCommand = "show";
const string pdfCommand = "generatePdf";
const string createFromLastCommand = "createFromLast";

const string selectParam = "select";
const string valueParam = "value";
const string dateParam = "date";

const std::vector<string> selectTypes = {"first", "last", "id"};

void setupCommandLine(CommandLineManager& cli)
{
   cli.AddParameter(dbCommand, "Invoice Database to use");

   // Actions
   cli.AddParameter(listCommand, "List all invoices in the database");
   cli.AddParameter(showCommand, "Display a single invoice in the database");
   cli.AddParameter(pdfCommand, "Generates a PDF file of the selected invoice");
   cli.AddParameter(createFromLastCommand, "Creates a new invoice that is a copy of the last invoice,\n"
                                           "but at the specified date");

   // Data parameters for actions
   cli.AddParameter(selectParam, "Used to select a single invoice in the database.\n"
                                 "Used by show and generatePdf commands. Values can be :\n"
                                 "  first : selects the first invoice in the Dababase (by ID)\n"
                                 "  last : selects the last invoice in the Dababase (by ID)\n"
                                 "  id : selects the invoice in the Dababase with the specified ID");
   cli.AddParameter(valueParam, "Value to use for the select type. Can be Id, Name... whatever was\n"
                                "specified in by the type parameter.");
   cli.AddParameter("template", "Filename template to use when generating a PDF file");
   cli.AddParameter(dateParam, "Date of the new invoice. Values can be :\n"
                               "  today : uses today as the date of the new invoice.\n"
                               "  lastDayOfThisMonth : uses the last day of this month.\n"
                               "  dd/mm/yyy : uses the date specified by this format.");

   cli.EnableHelpCommand();

   const string appName = "Invoice manager Command line Interface";
   const string appVersion = "1.0";
   const string author = "Mickaël da Cruz Guimarães";
   const string copyrightInfo = "2023";
   cli.EnableVersionCommand(appName, appVersion, author, copyrightInfo);
}

void runListCommand(const InvoiceDbController& controller)
{
   const std::vector<InvoiceUserData> allData = controller.getAllInvoiceTemplateData();

   cout << allData.size() << " invoices in database." << endl;
   InvoicePrinter::printMultiple(allData);
}

void runCreateFromLastCommand(InvoiceDbController& controller, const QDate& date)
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

void runShowCommand(const InvoiceDbController& controller, const int invoiceId)
{
   if (!controller.invoiceExists(invoiceId))
   {
      cout << "There is no invoice with id " << invoiceId << " in the database." << endl;
      return;
   }

   const InvoiceUserData data = controller.getInvoiceUserData(invoiceId);
   InvoicePrinter::printSingle(data);
}

QDate GetLastDayOfThisMonth()
{
   QDate today = QDate::currentDate();
   return QDate(today.year(), today.month(), today.daysInMonth());
}

void executeCommands(CommandLineManager& cli)
{
   const bool isHelp = cli.HandleHelpCommand();
   const bool isVersion = cli.HandleVersionCommand();
   const bool hasUnknownParams = cli.HandleUnknownParameters();
   if (!isHelp && !isVersion && !hasUnknownParams)
   {
      if (!cli.HasParameter(dbCommand))
         std::cout << "No Database provided, no data to run commands on." << std::endl;
      const string dbFile = cli.GetParameterValue(dbCommand);

      InvoiceDbController controller;
      const bool ok = controller.openDb(QString::fromStdString(dbFile));
      if (!ok)
         std::cout << "Error opening database " << dbFile << std::endl;

      if (cli.HasParameter(listCommand))
         runListCommand(controller);
      else if (cli.HasParameter(showCommand))
      {
         const string selectType = cli.GetParameterValue(selectParam);
         if (selectType.empty())
            cout << "Error : No selection type provided." << endl;
         else if (std::find(selectTypes.begin(), selectTypes.end(), selectType) == selectTypes.end())
            cout << " Error : unknown selection type " << selectType << endl;
         else
         {
            int invoiceId = -1;
            if (selectType == "first")
               invoiceId = controller.getFirstInvoiceId();
            else if (selectType == "last")
               invoiceId = controller.getLastInvoiceId();
            else if (selectType == "id")
            {
               const string valueStr = cli.GetParameterValue("value");
               if (valueStr.empty())
                  cout << "Error : No value specified. Required for select " << selectType << endl;
               else
               {
                  try {
                     invoiceId = std::stoi(valueStr);
                  }  catch (...) {
                     cout << "Error : " << valueStr << " is not a valid value for an id." << endl;
                  }
               }
            }

            if (invoiceId > -1)
               runShowCommand(controller, invoiceId);
         }
      }
      else if (cli.HasParameter(pdfCommand))
         ;// run pdf cmd
      else if (cli.HasParameter(createFromLastCommand))
      {
         QDate selectedDate;
         const string dateValue = cli.GetParameterValue(dateParam);
         cout << "Date param : " << dateValue << endl;
         if (dateValue == "today")
            selectedDate = QDate::currentDate();
         else if (dateValue == "lastDayOfThisMonth")
            selectedDate = GetLastDayOfThisMonth();
         else
         {
            if (dateValue.empty())
            {
               cout << "No date provided. Using last day of this month." << endl;
               selectedDate = GetLastDayOfThisMonth();
            }
            else
               selectedDate = QDate::fromString(QString::fromStdString(dateValue), "DD/MM/yyyy");
         }
         if (selectedDate.isValid())
            runCreateFromLastCommand(controller, selectedDate);
         else
            cout << "Error : invalid date specified." << endl;
      }
   }
}

int main(int argc, char** argv)
{
   CommandLineManager cliManager(argc, argv);

   setupCommandLine(cliManager);
   executeCommands(cliManager);

   return 1;
}
