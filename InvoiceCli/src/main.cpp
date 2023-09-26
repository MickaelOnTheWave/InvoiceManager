#include <iostream>
#include <string>

#include "commandlinemanager.h"
#include "InvoiceDbController.h"

// TODO Make ToolsLib usable lib and use it here instead of using copied cpp/h

using std::string;
using std::cout;
using std::endl;

const std::string dbCommand = "db";
const std::string listCommand = "list";
const std::string showCommand = "show";
const std::string showLastCommand = "showlast";
const std::string pdfCommand = "generatePdf";

void setupCommandLine(CommandLineManager& cli)
{
   cli.AddParameter(dbCommand, "Invoice Database to use");

   // Actions
   cli.AddParameter(listCommand, "List all invoices in the database");
   cli.AddParameter(showCommand, "Display a single invoice in the database");
   cli.AddParameter(showLastCommand, "Display the last invoice in the database (by ID)");
   cli.AddParameter(pdfCommand, "Generates a PDF file of the selected invoice");

   // Data parameters for actions
   cli.AddParameter("select", "Used to select a single invoice in the database.\n"
                              "Used by show and generatePdf commands. Values can be :\n"
                              "  first : selects the first invoice in the Dababase (by ID)\n"
                              "  last : selects the last invoice in the Dababase (by ID)\n"
                              "  id [ID]: selects the invoice in the Dababase with the specified ID");
   cli.AddParameter("template", "Filename template to use when generating a PDF file");

   cli.EnableHelpCommand();

   const string appName = "Invoice manager Command line Interface";
   const string appVersion = "1.0";
   const string author = "Mickaël da Cruz Guimarães";
   const string copyrightInfo = "2023";
   cli.EnableVersionCommand(appName, appVersion, author, copyrightInfo);
}

void runListCommand(const InvoiceDbController& controller)
{


}

void runShowLastCommand(const InvoiceDbController& controller)
{
   const auto lastInvoiceData = controller.getLastInvoiceData();

   cout << "Last Invoice Data :" << endl;
   cout << "  Client id     : " << lastInvoiceData.clientId << endl;
   cout << "  Template id   : " << lastInvoiceData.templateId << endl;
   cout << "  Stylesheed id : " << lastInvoiceData.stylesheetId << endl;
   cout << "  Currency      : " << lastInvoiceData.currency.toStdString() << endl;
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
         ;// run show cmd
      else if (cli.HasParameter(showLastCommand))
         runShowLastCommand(controller);
      else if (cli.HasParameter(pdfCommand))
         ;// run pdf cmd
   }
}

int main(int argc, char** argv)
{
   CommandLineManager cliManager(argc, argv);

   setupCommandLine(cliManager);
   executeCommands(cliManager);

   return 1;
}
