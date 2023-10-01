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

#include "SelectCliParamHandler.h"

#include <iostream>
#include "CliParametersDefinitions.h"

using std::cout;
using std::endl;
using std::string;

int SelectCliParamHandler::GetId(const InvoiceDbController& controller, const CommandLineManager& cli)
{
   SelectCliParamHandler handler(controller, cli);
   return handler.GetId();
}

SelectCliParamHandler::SelectCliParamHandler(const InvoiceDbController& _controller, const CommandLineManager& _cli)
   : controller(_controller), cli(_cli)
{
}

int SelectCliParamHandler::GetId()
{
   const string selectType = cli.GetParameterValue(selectParam);
   if (selectType.empty())
      cout << "Error : No selection type provided." << endl;
   else if (std::find(selectTypes.begin(), selectTypes.end(), selectType) == selectTypes.end())
      cout << " Error : unknown selection type " << selectType << endl;
   else
   {
      const int invoiceId = GetInvoiceIdFromCli(selectType);
      if (!controller.invoiceExists(invoiceId))
         cout << "There is no invoice with id " << invoiceId << " in the database." << endl;
      else
         return invoiceId;
   }
   return -1;
}

int SelectCliParamHandler::GetInvoiceIdFromCli(const std::string& selectType)
{
   if (selectType == "first")
      return controller.getFirstInvoiceId();
   else if (selectType == "last")
      return controller.getLastInvoiceId();
   else if (selectType == "id")
   {
      const string valueStr = cli.GetParameterValue("value");
      if (valueStr.empty())
      {
         cout << "Error : No value specified. Required for select " << selectType << endl;
         return -1;
      }

      try {
         return std::stoi(valueStr);
      }  catch (...) {
         cout << "Error : " << valueStr << " is not a valid value for an id." << endl;
      }
   }
   return -1;
}
