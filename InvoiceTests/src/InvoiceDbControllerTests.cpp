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

void populateWithTestData(InvoiceDbController& controller)
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

   // Add invoices


}

/*******************/

TEST_CASE( "InvoiceDbController - getTotalClientCount" )
{
   InvoiceDbController controller;
   populateWithTestData(controller);


   SECTION("Child companies separated")
   {
      REQUIRE( controller.getTotalClientCount(true) == 4 );
   }

   SECTION("Child companies grouped")
   {
      REQUIRE( controller.getTotalClientCount(false) == 3 );
   }
}

/*******************/

