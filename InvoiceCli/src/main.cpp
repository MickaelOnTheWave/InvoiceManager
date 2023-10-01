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

#include "CliParametersDefinitions.h"
#include "CreateFromLastCommand.h"
#include "ShowCommand.h"

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

void runCreatePdfCommand(const InvoiceDbController& controller,
                         const CommandLineManager& cli)
{
   // run pdf cmd
}

void executeCommands(CommandLineManager& cli)
{
   const bool isHelp = cli.HandleHelpCommand();
   const bool isVersion = cli.HandleVersionCommand();
   const bool hasUnknownParams = cli.HandleUnknownParameters();
   if (isHelp || isVersion || hasUnknownParams)
      return;

   if (!cli.HasParameter(dbCommand))
   {
      std::cout << "No Database provided, no data to run commands on." << std::endl;
      return;
   }

   const string dbFile = cli.GetParameterValue(dbCommand);
   InvoiceDbController controller;
   const bool ok = controller.openDb(QString::fromStdString(dbFile));
   if (!ok)
   {
      std::cout << "Error opening database " << dbFile << std::endl;
      return;
   }

   if (cli.HasParameter(listCommand))
      runListCommand(controller);
   else if (cli.HasParameter(showCommand))
      ShowCommand::runShow(controller, cli);
   else if (cli.HasParameter(pdfCommand))
      runCreatePdfCommand(controller, cli);
   else if (cli.HasParameter(createFromLastCommand))
      CreateFromLastCommand::Run(controller, cli);
}

int main(int argc, char** argv)
{
   CommandLineManager cliManager(argc, argv);

   setupCommandLine(cliManager);
   executeCommands(cliManager);

   return 1;
}
