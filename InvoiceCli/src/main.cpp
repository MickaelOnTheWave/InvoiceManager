#include <algorithm>
#include <iostream>
#include <string>

#include "commandlinemanager.h"
#include "InvoiceDbController.h"

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
   addLineInformation(lineStr, QString::asprintf("%.2f", data), minPosition, maxPosition);
}

void printInvoiceOneLine(const InvoiceTemplateData& data)
{
   string oneLinerStr;
   addLineInformation(oneLinerStr, data.id, 1, 7);
   addLineInformation(oneLinerStr, data.clientCompany.name, 9, 44);
   addLineInformation(oneLinerStr, getTotalValue(data.details), 46, 57);
   addLineInformation(oneLinerStr, data.date.toString("dd/MM/yyyy"), 59, 75);
   cout << oneLinerStr << endl;
}

void runListCommand(const InvoiceDbController& controller)
{
   const std::vector<InvoiceTemplateData> allData = controller.getAllInvoiceTemplateData();

   cout << allData.size() << " invoices in database." << endl;

   printInvoiceListHeader();
   for (const auto& invoiceData : allData)
      printInvoiceOneLine(invoiceData);
   cout << "|--------------------------------------------------------------------------|" << endl;
}

void printInvoiceDetails(const InvoiceTemplateData& data)
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

void runCreateFromLastCommand(const InvoiceDbController& controller, const QDate& date)
{
   InvoiceTemplateData data = controller.getInvoiceTemplateData(controller.getLastUsedInvoiceId());

   ++data.id;
   data.date = date;
   cout << "Here are is the invoice that will be created :" << endl;
   printInvoiceDetails(data);

   //controller.(data);
}

void runShowCommand(const InvoiceDbController& controller, const int invoiceId)
{
   if (!controller.invoiceExists(invoiceId))
   {
      cout << "There is no invoice with id " << invoiceId << " in the database." << endl;
      return;
   }

   const InvoiceTemplateData data = controller.getInvoiceTemplateData(invoiceId);
   printInvoiceDetails(data);
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
               invoiceId = controller.getLastUsedInvoiceId();
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
