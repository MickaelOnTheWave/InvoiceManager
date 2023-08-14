#ifndef INVOICEDATA_H
#define INVOICEDATA_H

#include <vector>
#include <QDate>

#include "CompanyData.h"
#include "InvoiceDetail.h"

class InvoiceBaseData
{
public:
   std::vector<InvoiceDetail> details;
   QString notes = "";
   QString currency;
};

class InvoiceData : public InvoiceBaseData
{
public:
    InvoiceData();

    int clientId = -1;
    int templateId = -1;
    int stylesheetId = -1;
};

class InvoiceTemplateData : public InvoiceBaseData
{
public:
   QString templatePath;
   QString stylesheetPath;

   CompanyData userCompany;
   CompanyData clientCompany;
   QDate date;
   int id;
};

#endif // INVOICEDATA_H
