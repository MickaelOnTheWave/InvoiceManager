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

#include "TestUtilities.h"

TestUtilities::TestUtilities()
{
   CompanyData data;
   data.name = "Client Company SA";
   data.address = "12 Big Avenue.\n123-456 ANOTHER CITY\nCOUNTRY";
   data.email = "client@company.com";
   data.phoneNumber = "+99 442-456";
   clientCompanySamples.push_back(data);

   data.name = "Client Company New Name SA";
   data.address = "13 Big Avenue.\n123-456 ANOTHER CITY\nCOUNTRY";
   data.email = "client@new-company.com";
   data.phoneNumber = "+91 442-456";
   clientCompanySamples.push_back(data);

   data.name = "Some Other Client Gmbh";
   data.address = "56 Rasherwer Streetz.\n45721 SHNELL CITY\nGERMANY";
   data.email = "heidi@deutshland.de";
   data.phoneNumber = "+99 442-456";
   clientCompanySamples.push_back(data);

   data.name = "Ma Petite Entreprise SARL";
   data.address = "578 Avenue la Grande Entreprise.\n26400 BOURG LES CAPITALES\nFRANCE";
   data.email = "contact@entreprise.fr";
   data.phoneNumber = "+33 4 75 80 42 12";
   clientCompanySamples.push_back(data);
}

CompanyData TestUtilities::createUserCompanyData()
{
   CompanyData data;
   data.name = "My Company Inc.";
   data.address = "432 Somewhere Street.\n784-456 SOME CITY\nCOUNTRY";
   data.email = "myself@mycompany.com";
   data.phoneNumber = "+99 123-456";
   return data;
}

CompanyData TestUtilities::createClientCompanyData(const int index) const
{
   return clientCompanySamples[index];
}

