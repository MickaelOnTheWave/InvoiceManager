/*
 * InvoiceManager
 * Copyright (C) 2024 Guimarães Tecnologia Ltda
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

#include "ListCommand.h"

#include <iostream>

#include "InvoicePrinter.h"
#include "SelectCliParamHandler.h"

using std::cout;
using std::endl;

ListCommand::ListCommand(const std::string& cliParamName)
   : AbstractCommand(cliParamName)
{
}

void ListCommand::Run(InvoiceDbController& controller, const CommandLineManager& cli)
{
   const std::vector<InvoiceUserData> allData = controller.getAllInvoiceTemplateData();
   cout << allData.size() << " invoices in database." << endl;
   InvoicePrinter::printMultiple(allData);
}
