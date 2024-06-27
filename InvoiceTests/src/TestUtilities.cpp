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


using std::vector;

TestUtilities::TestUtilities()
{
   populateCompanySamples();
   populateInvoiceDetailsSamples();
   populateInvoiceSamples();
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

InvoiceDbData TestUtilities::createInvoiceData(const int index)
{
   return invoiceSamples[index];
}

vector<InvoiceDetail> TestUtilities::createInvoiceDetails(const int index, const int size)
{
   vector<InvoiceDetail> details;
   for (int i=0; i<size; ++i)
      details.push_back(invoiceDetailsSamples[index + i]);
   return details;
}

void TestUtilities::populateCompanySamples()
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

void TestUtilities::populateInvoiceSamples()
{
   InvoiceDbData data;
   data.notes = "Some description notes about the invoice";
   data.currency = "$";
   data.date = QDate(2023, 12, 10);
   data.id = 1;
   data.templateId = -1;
   data.stylesheetId = -1;
   data.clientId = 2;
   invoiceSamples.push_back(data);

   data.notes = "This invoice is also regarding first client";
   data.currency = "$";
   data.date = QDate(2023, 12, 14);
   data.id = 2;
   data.templateId = -1;
   data.stylesheetId = -1;
   data.clientId = 2;
   invoiceSamples.push_back(data);

   data.notes = "Nothing special to consider here";
   data.currency = "€";
   data.date = QDate(2024, 01, 03);
   data.id = 3;
   data.templateId = -1;
   data.stylesheetId = -1;
   data.clientId = 3;
   invoiceSamples.push_back(data);

   data.notes = "Another invoice";
   data.currency = "€";
   data.date = QDate(2024, 01, 20);
   data.id = 4;
   data.templateId = -1;
   data.stylesheetId = -1;
   data.clientId = 3;
   invoiceSamples.push_back(data);

   data.notes = "Some description notes about the invoice";
   data.currency = "€";
   data.date = QDate(2024, 01, 02);
   data.id = 5;
   data.templateId = -1;
   data.stylesheetId = -1;
   data.clientId = 4;
   invoiceSamples.push_back(data);

   data.notes = "";
   data.currency = "€";
   data.date = QDate(2024, 02, 01);
   data.id = 6;
   data.templateId = -1;
   data.stylesheetId = -1;
   data.clientId = 5;
   invoiceSamples.push_back(data);

   data.notes = "Facturé en Reals parce que oui";
   data.currency = "R$";
   data.date = QDate(2024, 07, 01);
   data.id = 7;
   data.templateId = -1;
   data.stylesheetId = -1;
   data.clientId = 5;
   invoiceSamples.push_back(data);
}

void TestUtilities::populateInvoiceDetailsSamples()
{
   invoiceDetailsSamples.push_back(InvoiceDetail("Consulting Service", 1.0, 8000.0));
   invoiceDetailsSamples.push_back(InvoiceDetail("Support Service", 3.0, 500.0));
   invoiceDetailsSamples.push_back(InvoiceDetail("Some Crazy Service", 1.5, 1500.0));

   invoiceDetailsSamples.push_back(InvoiceDetail("Consulting Service", 1.0, 5000.0));
   invoiceDetailsSamples.push_back(InvoiceDetail("Support Service", 1.0, 200.0));

   invoiceDetailsSamples.push_back(InvoiceDetail("Software development", 1.0, 4000.0));
   invoiceDetailsSamples.push_back(InvoiceDetail("Support Service", 1.0, 250.0));

   invoiceDetailsSamples.push_back(InvoiceDetail("Software development", 1.0, 2500.0));

   invoiceDetailsSamples.push_back(InvoiceDetail("Research and Development", 2.0, 5500.0));

   invoiceDetailsSamples.push_back(InvoiceDetail("Playing video games", 28.2, 12.78));
   invoiceDetailsSamples.push_back(InvoiceDetail("Product license", 2.0, 1000.0));
   invoiceDetailsSamples.push_back(InvoiceDetail("Some other product", 1.0, 500.0));

   invoiceDetailsSamples.push_back(InvoiceDetail("Recherche et développement", 3.0, 1500.0));
   invoiceDetailsSamples.push_back(InvoiceDetail("Glandouillage", 1.0, 18000.0));
   invoiceDetailsSamples.push_back(InvoiceDetail("Conseil", 1.0, 6500.0));
   invoiceDetailsSamples.push_back(InvoiceDetail("Autres services débiles", 3.0, 300.0));
}
