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

#include <catch2/catch.hpp>

#include "InvoiceData.h"
#include "InvoiceDetailsModel.h"
#include "InvoiceDocument.h"
#include "Utils.h"


InvoiceUserData createDefaultInvoiceData()
{
   InvoiceUserData data;
   data.notes = "Some description notes about the invoice";
   data.currency = "$";
   data.date = QDate(2023, 12, 10);
   data.id = 123;
   data.templateData = "template/path.html";
   data.stylesheetData = "stylesheet/path.css";

   data.userCompany.name = "My Company Inc.";
   data.userCompany.address = "432 Somewhere Street.\n784-456 SOME CITY\nCOUNTRY";
   data.userCompany.email = "myself@mycompany.com";
   data.userCompany.phoneNumber = "+99 123-456";

   data.clientCompany.name = "Client Company SA";
   data.clientCompany.address = "12 Big Avenue.\n123-456 ANOTHER CITY\nCOUNTRY";
   data.clientCompany.email = "client@company.com";
   data.clientCompany.phoneNumber = "+99 442-456";

   data.details.push_back(InvoiceDetail("Consulting Service", 1.0, 8000.0));
   data.details.push_back(InvoiceDetail("Support Service", 3.0, 500.0));
   data.details.push_back(InvoiceDetail("Some Crazy Service", 1.5, 1500.0));

   return data;
}

TEST_CASE( "InvoiceDocument", "First test" )
{
   InvoiceDocument invoiceDoc;
   invoiceDoc.setData(createDefaultInvoiceData());

   REQUIRE( invoiceDoc.GetInvoiceId() == 123 );
}

/*******************/

void populateInvoiceModel(InvoiceDetailsModel* model)
{
   model->addTotalRow();
   model->addDetail("Some Service", 123.4, 2);
   model->addDetail("Other Service", 765.2, 4.5);
}

double getModelValue(InvoiceDetailsModel* model, const int row, const int column)
{
   return model->data(model->index(row, column)).toDouble();
}

TEST_CASE( "InvoiceDetailsModel", "computeTotals" )
{
   auto model = new InvoiceDetailsModel();
   populateInvoiceModel(model);
   model->computeTotals();

   REQUIRE_THAT( getModelValue(model, 0, 3), Catch::Matchers::WithinAbs(246.8, 0.00001) );
   REQUIRE_THAT( getModelValue(model, 1, 3), Catch::Matchers::WithinAbs(3443.4, 0.00001) );
   REQUIRE_THAT( getModelValue(model, 2, 1), Catch::Matchers::WithinAbs(0.0, 0.00001) );
   REQUIRE_THAT( getModelValue(model, 2, 2), Catch::Matchers::WithinAbs(0.0, 0.00001) );
   REQUIRE_THAT( getModelValue(model, 2, 3), Catch::Matchers::WithinAbs(3690.2, 0.00001) );
}
