#include <algorithm>
#include <iostream>
#include <string>

#include "commandlinemanager.h"
#include "InvoiceDbController.h"

// TODO Make ToolsLib usable lib and use it here instead of using copied cpp/h
// TODO use same version in installer, cli, gui and everywhere

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

void printInvoiceListHeader()
{
   cout << "|--------------------------------------------------------------------------|" << endl;
   cout << "|  id  |                 Client             | Total value |      Date      |" << endl;
   cout << "|--------------------------------------------------------------------------|" << endl;
}

double getTotalValue(const std::vector<InvoiceDetail> details)
{
   auto operation = [] (double sum, const InvoiceDetail& detail) {
      return sum + detail.value;
   };
   return std::accumulate(details.begin(), details.end(), 0.0, operation);
}

// TODO Remove duplication with commandlinemanager.cpp
string Spaces(const int spaceCount)
{
   string spaceStr;
   for (int i=0; i<spaceCount; ++i)
      spaceStr += " ";
   return spaceStr;
}

void addLineInformation(string& lineStr, const string& data, const int minPosition, const int maxPosition)
{
   const int spacesToAdd = minPosition - lineStr.length();
   if (spacesToAdd > 0)
      lineStr += Spaces(spacesToAdd);

   const int maxDataLength = maxPosition - minPosition;
   const string dataToUse = (data.length() > maxDataLength) ? data.substr(0, maxDataLength) : data;
   lineStr += dataToUse;
}

void addLineInformation(string& lineStr, const QString data, const int minPosition, const int maxPosition)
{
   addLineInformation(lineStr, data.toStdString(), minPosition, maxPosition);
}

void addLineInformation(string& lineStr, const int data, const int minPosition, const int maxPosition)
{
   addLineInformation(lineStr, QString::number(data), minPosition, maxPosition);
}

void addLineInformation(string& lineStr, const double data, const int minPosition, const int maxPosition)
{
   addLineInformation(lineStr, QString::number(data), minPosition, maxPosition);
}

void printInvoiceOneLine(const InvoiceTemplateData& data)
{
   string oneLinerStr;
   addLineInformation(oneLinerStr, data.id, 1, 7);
   addLineInformation(oneLinerStr, data.clientCompany.name, 9, 44);
   addLineInformation(oneLinerStr, getTotalValue(data.details), 46, 57);
   addLineInformation(oneLinerStr, data.date.toString(), 59, 75);
   cout << oneLinerStr << endl;
}

void runListCommand(const InvoiceDbController& controller)
{
   const std::vector<InvoiceTemplateData> allData = controller.getAllInvoiceTemplateData();

   cout << allData.size() << " invoices in database." << endl;

   printInvoiceListHeader();
   for (const auto& invoiceData : allData)
      printInvoiceOneLine(invoiceData);
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
