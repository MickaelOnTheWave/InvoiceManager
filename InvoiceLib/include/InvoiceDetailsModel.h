#ifndef INVOICEDETAILSMODEL_H
#define INVOICEDETAILSMODEL_H

#include <QStandardItemModel>

#include "InvoiceDetail.h"

class InvoiceDetailsModel : public QStandardItemModel
{
   Q_OBJECT
public:
   InvoiceDetailsModel();

   void computeTotals();
   void addDetail(const QString &name, const double value, const double quantity);
   void addTotalRow();
   std::vector<InvoiceDetail> toDetailsCollection() const;


private:
   QVariant GetInvoiceDetailData(const int row, const int column) const;
   void SetInvoiceDetailData(const int row, const int column, const QVariant& data);

};

#endif // INVOICEDETAILSMODEL_H
