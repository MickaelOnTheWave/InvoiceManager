#ifndef INVOICESORTPROXYMODEL_H
#define INVOICESORTPROXYMODEL_H

#include <QSortFilterProxyModel>

class InvoiceSortProxyModel : public QSortFilterProxyModel
{
public:
   explicit InvoiceSortProxyModel(QObject *parent = nullptr);

protected:
   bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
   bool dateLessThan(const QModelIndex& left, const QModelIndex& right) const;
   bool dateLessThan(const QVariant& left, const QVariant& right) const;

   static bool isEqual(const double left, const double right);
};

#endif // INVOICESORTPROXYMODEL_H
