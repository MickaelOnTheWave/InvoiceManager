#include "InvoiceModel.h"

InvoiceModel::InvoiceModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    refresh();
}

void InvoiceModel::refresh()
{
    const int myCompanyId = 1;
    const QString queryStr = "SELECT company.name AS 'Client',"
                             "invoice.id AS 'Amount',"
                             "stylesheet.name AS 'Stylesheet',"
                             "invoice.date as 'Date' "
                             "FROM invoice, company, stylesheet "
                             "WHERE invoice.clientId = company.id "
                             "AND invoice.stylesheetId = stylesheet.id "
                             "AND invoice.companyId = %1 "
                             "ORDER BY invoice.date";
    setQuery(queryStr.arg(myCompanyId));
}
