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

#include <QApplication>

#include "commandlinemanager.h"
#include "InvoiceDbController.h"
#include "InvoiceDocument.h"
#include "InvoicePrinter.h"

#include "CliParametersDefinitions.h"
#include "Commands/CreateFromLastCommand.h"
#include "Commands/CreatePdfCommand.h"
#include "Commands/InvoiceCommands.h"
#include "Commands/ListCommand.h"
#include "DbMigrator.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;

const string dbParam = "db";
const string listCommand = "list";
const string showCommand = "show";
const string pdfCommand = "generatePdf";
const string createFromLastCommand = "createFromLast";
const string getHtmlCommand = "getHtml";
const string getCssCommand = "getCss";
const string getStyledHtmlCommand = "getStyledHtml";


void setupCommandLine(CommandLineManager& cli)
{
   cli.AddParameter(dbParam, "Invoice Database to use");

   // Actions
   cli.AddParameter(listCommand, "List all invoices in the database");
   cli.AddParameter(showCommand, "Display a single invoice in the database");
   cli.AddParameter(pdfCommand, "Generates a PDF file of the selected invoice");
   cli.AddParameter(createFromLastCommand, "Creates a new invoice that is a copy of the last invoice,\n"
                                           "but at the specified date");
   cli.AddParameter(getHtmlCommand, "Returns the HTML content of the selected invoice.");
   cli.AddParameter(getCssCommand, "Returns the CSS file path for the selected invoice.");
   cli.AddParameter(getStyledHtmlCommand, "Returns the HTML content with embedded CSS of the selected invoice.");

   // Data parameters for actions
   cli.AddParameter(selectParam, "Used to select a single invoice in the database.\n"
                                 "Used by show and generatePdf commands. Values can be :\n"
                                 "  first : selects the first invoice in the Dababase (by ID)\n"
                                 "  last : selects the last invoice in the Dababase (by ID)\n"
                                 "  id : selects the invoice in the Dababase with the specified ID"
                                 "  date : selects the first invoice in the Dababase that has the specified date");
   cli.AddParameter(valueParam, "Value to use for the select type. Can be Id, Name... whatever was\n"
                                "specified in by the type parameter.");
   cli.AddParameter(namePatternParam, "Filename template to use when generating a PDF file\n"
                                      "Available parameters are :\n"
                                      "  [YYYY] : the year number\n"
                                      "  [MM]   : the month number\n"
                                      "  [DD]   : the day of month number\n"
                                      "  [CLIENT] : the client name\n"
                                      "  [ID]   : the invoice Id\n"
                                      "All other characters are literal.");
   cli.AddParameter(dateParam, "Date of the new invoice. Values can be :\n"
                               "  today : uses today as the date of the new invoice.\n"
                               "  lastDayOfThisMonth : uses the last day of this month.\n"
                               "  dd/mm/yyy : uses the date specified by this format.");

   cli.EnableHelpCommand();

   const string appName = "Invoice manager Command line Interface";
   const string author = "Mickaël da Cruz Guimarães";
   const string copyrightInfo = "2023";
   cli.EnableVersionCommand(appName, APP_VERSION, author, copyrightInfo);
}

void executeKnownCommands(CommandLineManager& cli, InvoiceDbController& controller)
{
   const vector<AbstractCommand*> knownCommands = {
      new ListCommand(listCommand),
      new CreatePdfCommand(pdfCommand),
      new CreateFromLastCommand(createFromLastCommand),
      new ShowCommand(showCommand),
      new GetHtmlCommand(getHtmlCommand),
      new GetCssCommand(getCssCommand),
      new GetStyledHtmlCommand(getStyledHtmlCommand)
   };

   for (auto command : knownCommands)
   {
      if (cli.HasParameter(command->GetCliParamName()))
      {
         command->Run(controller, cli);
         break;
      }
   }
}

bool askUserForDbMigration(InvoiceDbController& controller)
{
   std::cout << "DB version mismatch. DB version : %1 - Current version : %2" << std::endl;
   std::cout << "No action can be performed on this DB until it is migrated." << std::endl;
   std::cout << "Do you want to migrate the DB file ? (type y for Yes, n for No)" << std::endl;

   std::string userAnswer;
   std::cin >> userAnswer;
   return (userAnswer == "y" || userAnswer == "Y" || userAnswer == "YES" ||
           userAnswer == "yes" || userAnswer == "Yes" || userAnswer == "YES");
}

void executeCommands(CommandLineManager& cli)
{
   const bool isHelp = cli.HandleHelpCommand();
   const bool isVersion = cli.HandleVersionCommand();
   const bool hasUnknownParams = cli.HandleUnknownParameters();
   if (isHelp || isVersion || hasUnknownParams)
      return;

   if (!cli.HasParameter(dbParam))
   {
      std::cout << "No Database provided, no data to run commands on." << std::endl;
      return;
   }

   const string dbFile = cli.GetParameterValue(dbParam);
   InvoiceDbController controller;
   const bool ok = controller.openDb(QString::fromStdString(dbFile));
   if (!ok)
   {
      std::cout << "Error opening database " << dbFile << std::endl;
      return;
   }

   const int dbVersion = controller.getDatabaseVersion();
   if (dbVersion != controller.currentDbVersion)
   {
      const bool shouldMigrateDb = askUserForDbMigration(controller);
      if (shouldMigrateDb)
      {
         DbMigrator migrator(controller);
         const bool ok = migrator.Migrate();
         if (!ok)
         {
            std::cout << "Error trying to migrate the DB. Closing." << std::endl;
            return ;
         }
      }
      else
      {
         std::cout << "Ok. Exiting." << std::endl;
         return;
      }
   }

   executeKnownCommands(cli, controller);
}

int main(int argc, char** argv)
{
   QApplication application(argc, argv);
   CommandLineManager cliManager(argc, argv);

   setupCommandLine(cliManager);
   executeCommands(cliManager);

   return 1;
}
