#include "InvoiceDetailsModel.h"

InvoiceDetailsModel::InvoiceDetailsModel() : QStandardItemModel()
{
   setColumnCount(4);
   setHeaderData(0, Qt::Horizontal, tr("Service"));
   setHeaderData(1, Qt::Horizontal, tr("Unitary Value"));
   setHeaderData(2, Qt::Horizontal, tr("Quantity"));
   setHeaderData(3, Qt::Horizontal, tr("Value"));
}

void InvoiceDetailsModel::computeTotals()
{
   double total = 0.0;
   for (int i=0; i<rowCount()-1; ++i)
   {
     const double quantity = GetInvoiceDetailData(i, 1).toDouble();
     const double unitaryValue = GetInvoiceDetailData(i, 2).toDouble();
     const int rowTotal = quantity * unitaryValue;
     SetInvoiceDetailData(i, 3, rowTotal);
     setData(index(rowCount()-1, 3), total);
     total += data(index(i, 3)).toDouble();
   }
   SetInvoiceDetailData(rowCount()-1, 3, total);
}

void InvoiceDetailsModel::addDetail(const QString &name, const double value, const double quantity)
{
    const int newRowIndex = rowCount()-1;
    insertRow(newRowIndex);
    SetInvoiceDetailData(newRowIndex, 0, name);
    SetInvoiceDetailData(newRowIndex, 1, quantity);
    SetInvoiceDetailData(newRowIndex, 2, value);
    SetInvoiceDetailData(newRowIndex, 3, quantity * value);
}

std::vector<InvoiceDetail> InvoiceDetailsModel::toDetailsCollection() const
{
    std::vector<InvoiceDetail> details;
    const int detailCount = rowCount() - 1;
    for (int i=0; i<detailCount; ++i)
    {
        const QString description = GetInvoiceDetailData(i, 0).toString();
        const double unitaryValue = GetInvoiceDetailData(i, 1).toDouble();
        const double quantity = GetInvoiceDetailData(i, 2).toDouble();
        details.emplace_back(description, quantity, unitaryValue);
    }
    return details;
}

QVariant InvoiceDetailsModel::GetInvoiceDetailData(const int row, const int column) const
{
   return data(index(row, column));
}

void InvoiceDetailsModel::SetInvoiceDetailData(const int row, const int column, const QVariant& data)
{
   setData(index(row, column), data);
}
