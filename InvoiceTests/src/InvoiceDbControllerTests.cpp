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


#include "TestUtilities.h"


#define private public
#include "IdParentingMap.h"
#include "InvoiceDbController.h"
#undef private

/*******************/

const char* dbFilename = "autotestDb.idb";

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

TEST_CASE( "InvoiceDbController - groupCompanyResults" )
{
   // Co 1 : 1
   // Co 2 : 2
   // Co 1 Ren : 3 (childId : 1)
   // Co 1 Ren2 : 4 (childId : 3)
   // Co 1 Ren3 : 5 (childId : 4)
   InvoiceDbController::IncomePerClientId incomeData = {
      {1, 1000.0},
      {2, 1200.0},
      {3, 300.0},
      {4, 400.0},
      {5, 500.0}
   };

   IdParentingMap parentingMap = { {5, {1, 3, 4}} };

   InvoiceDbController::groupCompanyResults(parentingMap, incomeData);
   REQUIRE( incomeData.size() == 2 );
   REQUIRE_THAT( incomeData[2], Catch::Matchers::WithinAbs(1200.0, 0.001));
   REQUIRE_THAT( incomeData[5], Catch::Matchers::WithinAbs(2200.0, 0.001));
}

TEST_CASE( "InvoiceDbController - createFinalParentMap" )
{
   // Co 1 : 1
   // Co 2 : 2
   // Co 1 Ren : 3 (childId : 1)
   // Co 1 Ren2 : 4 (childId : 3)
   // Co 1 Ren3 : 5 (childId : 4)
   CompanyChildMap idMap;
   idMap[3] = 1;
   idMap[4] = 3;
   idMap[5] = 4;

   IdParentingMap parentingMap = InvoiceDbController::createFinalParentMap(idMap);

   REQUIRE( parentingMap.size() == 1 );
   REQUIRE( parentingMap[5][0] == 4 );
   REQUIRE( parentingMap[5][1] == 3 );
   REQUIRE( parentingMap[5][2] == 1 );
}

TEST_CASE( "InvoiceDbController - getIncomePerClient" )
{
   InvoiceDbController controller;
   populateWithCompaniesAndInvoices(controller);


   std::map<QString, double> expectedResults;
   InvoiceDbController::IncomePerClientVec result;

   TestUtilities testUtils;
   SECTION("Child companies separated")
   {
      result = controller.getIncomePerClient(true);
      REQUIRE( result.size() == 4 );

      const double totalInvoice1 = 8000 + 3 * 500 + 1.5 * 1500;
      const double totalInvoice2 = 5200;
      double expectedTotal = totalInvoice1 + totalInvoice2;
      expectedResults[testUtils.createClientCompanyData(0).name] = expectedTotal;

      const double totalInvoice3 = 4250;
      const double totalInvoice4 = 2500;
      expectedTotal = totalInvoice3 + totalInvoice4;
      expectedResults[testUtils.createClientCompanyData(1).name] = expectedTotal;

      expectedTotal = 11000;
      expectedResults[testUtils.createClientCompanyData(2).name] = expectedTotal;

      const double totalInvoice6 = 28.2 * 12.78 + 2000 + 500;
      const double totalInvoice7 = 3 * 1500 + 18000 + 6500 + 3 * 300;
      expectedTotal = totalInvoice6 + totalInvoice7;
      expectedResults[testUtils.createClientCompanyData(3).name] = expectedTotal;
   }

   SECTION("Child companies grouped")
   {
      result = controller.getIncomePerClient(false);
      REQUIRE( result.size() == 3 );

      const double totalInvoice1 = 8000 + 3 * 500 + 1.5 * 1500;
      const double totalInvoice2 = 5200;
      const double totalInvoice3 = 4250;
      const double totalInvoice4 = 2500;
      double expectedTotal = totalInvoice1 + totalInvoice2 + totalInvoice3 + totalInvoice4;
      expectedResults[testUtils.createClientCompanyData(1).name] = expectedTotal;

      expectedTotal = 11000;
      expectedResults[testUtils.createClientCompanyData(2).name] = expectedTotal;

      const double totalInvoice6 = 28.2 * 12.78 + 2000 + 500;
      const double totalInvoice7 = 3 * 1500 + 18000 + 6500 + 3 * 300;
      expectedTotal = totalInvoice6 + totalInvoice7;
      expectedResults[testUtils.createClientCompanyData(3).name] = expectedTotal;
   }

   SECTION("Several chained grouped child companies")
   {

   }

   for (const auto& resultData : result)
   {
      auto itExpected = expectedResults.find(resultData.first);
      REQUIRE( itExpected != expectedResults.end());
      REQUIRE_THAT( resultData.second, Catch::Matchers::WithinAbs(itExpected->second, 0.00001));
   }

}

/*******************/

