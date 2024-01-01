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

#include "CreatePdfCommand.h"

#include <iostream>

#include "CliParametersDefinitions.h"
#include "InvoiceDocument.h"
#include "SelectCliParamHandler.h"

using std::cout;
using std::endl;

CreatePdfCommand::CreatePdfCommand(const std::string& cliParamName)
   : AbstractCommand(cliParamName)
{
}

void CreatePdfCommand::Run(InvoiceDbController& controller, const CommandLineManager& cli)
{
   std::string namePattern = cli.GetParameterValue(namePatternParam);
   if (namePattern.empty())
   {
      namePattern = "[YYYY]-[MM]-[DD] [CLIENT].pdf";
      cout << "No name pattern provided. Using \"" << namePattern << "\"." << endl;
   }

   const int invoiceId = SelectCliParamHandler::GetId(controller, cli);
   if (invoiceId > -1)
   {
      const InvoiceUserData invoiceData = controller.getInvoiceUserData(invoiceId);
      InvoiceDocument document;
      document.setData(invoiceData);

      document.CreatePdfFileFromPattern(QString::fromStdString(namePattern));
   }
}
