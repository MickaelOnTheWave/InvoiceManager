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

#include <catch2/catch.hpp>
#include <QSqlQuery>

#include "InvoiceDbController.h"
#include "TestUtilities.h"

/*******************/

const QString dbFilename = "autotestDb.db";

bool writeClientCompany(InvoiceDbController& controller, const CompanyData& data)
{
   const bool isClient = true;
   QSqlQuery query = controller.createWriteCompanyQuery(data, isClient);
   return query.exec();
}

void populateWithCompanies(InvoiceDbController& controller)
{
   bool ok = controller.createDb(dbFilename);
   REQUIRE ( ok == true );

   TestUtilities testUtils;
   // Add user company
   ok = controller.writeUserCompany(testUtils.createUserCompanyData());
   REQUIRE ( ok == true );

   // Add client companies
   for (int i=0; i<3; ++i)
   {
      ok = writeClientCompany(controller, testUtils.createClientCompanyData(i));
      REQUIRE ( ok == true );
   }

   ok = controller.writeUpdatedCompany(testUtils.createClientCompanyData(3), 2);
   REQUIRE ( ok == true );
}

void populateWithCompaniesAndInvoices(InvoiceDbController& controller)
{
   populateWithCompanies(controller);

   const std::vector<int> invoiceDetailsSizes = { 3, 2, 2, 1, 1, 3, 4};
   TestUtilities testUtils;
   // Add invoices
   int iDetails = 0;
   for (int i=0; i<7; ++i)
   {
      const std::vector<int> detailsIds = controller.writeInvoiceDetails(testUtils.createInvoiceDetails(iDetails, invoiceDetailsSizes[i]));
      REQUIRE ( detailsIds.size() > 0 );

      InvoiceDbData invoiceData = testUtils.createInvoiceData(i);
      invoiceData.detailsIds = detailsIds;
      bool ok = controller.writeInvoice(invoiceData);
      REQUIRE ( ok == true );
      iDetails += invoiceDetailsSizes[i];
   }
}


/*******************/

TEST_CASE( "InvoiceDbController - getTotalClientCount" )
{
   InvoiceDbController controller;
   populateWithCompanies(controller);


   SECTION("Child companies separated")
   {
      REQUIRE( controller.getTotalClientCount(true) == 4 );
   }

   SECTION("Child companies grouped")
   {
      REQUIRE( controller.getTotalClientCount(false) == 3 );
   }
}

TEST_CASE( "InvoiceDbController - getIncomePerClient" )
{
   InvoiceDbController controller;
   populateWithCompaniesAndInvoices(controller);


   TestUtilities testUtils;
   SECTION("Child companies separated")
   {
      const InvoiceDbController::IncomePerClientVec result = controller.getIncomePerClient();
      REQUIRE( result.size() == 4 );

      std::pair<QString, double> resultData = result[0];
      CompanyData sampleData = testUtils.createClientCompanyData(0);
      REQUIRE( resultData.first ==  sampleData.name );
      const double totalInvoice1 = 8000 + 3 * 500 + 1.5 * 1500;
      const double totalInvoice2 = 5200;
      double expectedTotal = totalInvoice1 + totalInvoice2;
      REQUIRE_THAT( resultData.second, Catch::Matchers::WithinAbs(expectedTotal, 0.00001));

      resultData = result[1];
      sampleData = testUtils.createClientCompanyData(1);
      REQUIRE( resultData.first ==  sampleData.name );
      const double totalInvoice3 = 4250;
      const double totalInvoice4 = 2500;
      expectedTotal = totalInvoice1 + totalInvoice2 + totalInvoice3;
      REQUIRE_THAT( resultData.second, Catch::Matchers::WithinAbs(expectedTotal, 0.00001));

      resultData = result[2];
      const double totalInvoice5 = 11000;
      sampleData = testUtils.createClientCompanyData(2);
      REQUIRE( resultData.first ==  sampleData.name );
      REQUIRE_THAT( resultData.second, Catch::Matchers::WithinAbs(totalInvoice5, 0.00001));

      resultData = result[3];
      const double totalInvoice6 = 28.2 * 12.78 + 2000 + 500;
      const double totalInvoice7 = 3 * 1500 + 18000 + 6500;
      sampleData = testUtils.createClientCompanyData(3);
      REQUIRE( resultData.first ==  sampleData.name );
      REQUIRE_THAT( resultData.second, Catch::Matchers::WithinAbs(totalInvoice6 + totalInvoice7, 0.00001));
   }

   SECTION("Child companies grouped")
   {
      const InvoiceDbController::IncomePerClientVec result = controller.getIncomePerClient();
      REQUIRE( result.size() == 3 );
   }
}

/*******************/

