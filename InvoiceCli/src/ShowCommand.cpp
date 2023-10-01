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

#include "ShowCommand.h"

#include <iostream>

#include "CliParametersDefinitions.h"
#include "InvoicePrinter.h"
#include "SelectCliParamHandler.h"

using std::string;
using std::cout;
using std::endl;

void ShowCommand::Run(const InvoiceDbController& controller, const CommandLineManager& cli)
{
   const int invoiceId = SelectCliParamHandler::GetId(controller, cli);
   if (invoiceId > -1)
   {
      const InvoiceUserData data = controller.getInvoiceUserData(invoiceId);
      InvoicePrinter::printSingle(data);
   }
}

