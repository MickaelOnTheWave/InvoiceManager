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

#include "InvoiceCommands.h"

#include <iostream>

#include "InvoiceDocument.h"
#include "InvoicePrinter.h"
#include "SelectCliParamHandler.h"

using std::cout;
using std::endl;

InvoiceCommand::InvoiceCommand(const std::string& cliParamName)
   : AbstractCommand(cliParamName)
{
}

void InvoiceCommand::Run(InvoiceDbController& controller, const CommandLineManager& cli)
{
   const int invoiceId = SelectCliParamHandler::GetId(controller, cli);
   if (invoiceId > -1)
      RunOnInvoice(invoiceId, controller);
}

ShowCommand::ShowCommand(const std::string& cliParamName)
   : InvoiceCommand(cliParamName)
{
}

void ShowCommand::RunOnInvoice(const int id, const InvoiceDbController& controller)
{
   const InvoiceUserData data = controller.getInvoiceUserData(id);
   InvoicePrinter::printSingle(data);
}

GetHtmlCommand::GetHtmlCommand(const std::string& cliParamName)
   : InvoiceCommand(cliParamName)
{
}

void GetHtmlCommand::RunOnInvoice(const int id, const InvoiceDbController& controller)
{
   InvoiceDocument invoiceDoc;
   invoiceDoc.setData(controller.getInvoiceUserData(id));
   const QString htmlContent = invoiceDoc.CreateHtmlContent();
   cout << htmlContent.toStdString() << endl;
}

GetCssCommand::GetCssCommand(const std::string& cliParamName)
   : InvoiceCommand(cliParamName)
{
}

void GetCssCommand::RunOnInvoice(const int id, const InvoiceDbController& controller)
{
   const InvoiceUserData data = controller.getInvoiceUserData(id);
   cout << data.stylesheetData.toStdString() << endl;
}

GetStyledHtmlCommand::GetStyledHtmlCommand(const std::string& cliParamName)
   : InvoiceCommand(cliParamName)
{
}

void GetStyledHtmlCommand::RunOnInvoice(const int id, const InvoiceDbController& controller)
{
   InvoiceDocument invoiceDoc;
   invoiceDoc.setData(controller.getInvoiceUserData(id));
   const QString htmlContent = invoiceDoc.CreateStyledHtmlContent();
   cout << htmlContent.toStdString() << endl;
}
