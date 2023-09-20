#include "InvoiceModel.h"

#include <QLocale>

InvoiceModel::InvoiceModel(QObject *parent)
    : QSqlQueryModel(parent)
{
   refresh();
}

QVariant InvoiceModel::data(const QModelIndex& item, int role) const
{
   QVariant targetData = QSqlQueryModel::data(item, role);

   const int amountColumn = 2;
   if (role == Qt::DisplayRole && item.column() == amountColumn && targetData.isValid())
   {
      const double amountValue = targetData.toDouble();
      return QVariant(QLocale(QLocale::English).toString(amountValue, 'f', 2));
   }
   return targetData;
}

void InvoiceModel::refresh()
{
    const int myCompanyId = 1;
    const QString sumQuery = "SELECT SUM(value) as Amount FROM invoiceelement WHERE id IN "
                             "(SELECT idElement FROM invoicedetailmap WHERE idInvoice = invoice.id )";
    const QString queryStr = "SELECT invoice.id,"
                             "company.name AS 'Client',"
                             "(%1) AS 'Amount',"
                             "stylesheet.name AS 'Stylesheet',"
                             "invoice.date as 'Date' "
                             "FROM invoice, company, stylesheet "
                             "WHERE invoice.clientId = company.id "
                             "AND invoice.stylesheetId = stylesheet.id "
                             "AND invoice.companyId = %2 "
                             "ORDER BY invoice.date";
    setQuery(queryStr.arg(sumQuery).arg(myCompanyId));
}
