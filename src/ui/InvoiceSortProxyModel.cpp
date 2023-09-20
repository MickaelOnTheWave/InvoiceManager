#include "InvoiceSortProxyModel.h"

#include <QDate>

InvoiceSortProxyModel::InvoiceSortProxyModel(QObject *parent)
   : QSortFilterProxyModel{parent}
{

}

bool InvoiceSortProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
   const QVariant leftData = sourceModel()->data(left);
   const QVariant rightData = sourceModel()->data(right);

   const int amountColumn = 2;
   const int dateColumn = 4;
   if (left.column() == amountColumn)
   {
      const double leftDouble = leftData.toDouble();
      const double rightDouble = rightData.toDouble();
      if (isEqual(leftDouble, rightDouble))
         return dateLessThan(left, right);
      return leftDouble < rightDouble;
   }
   else if (left.column() == dateColumn)
      return dateLessThan(leftData, rightData);
   else if (leftData == rightData)
      return dateLessThan(left, right);
   return QSortFilterProxyModel::lessThan(left, right);
}

bool InvoiceSortProxyModel::dateLessThan(const QModelIndex& left, const QModelIndex& right) const
{
   const int dateColumn = 4;
   const QModelIndex leftDateIndex = sourceModel()->index(left.row(), dateColumn);
   const QModelIndex rightDateIndex = sourceModel()->index(right.row(), dateColumn);
   return dateLessThan(sourceModel()->data(leftDateIndex), sourceModel()->data(rightDateIndex));
}

bool InvoiceSortProxyModel::dateLessThan(const QVariant& left, const QVariant& right) const
{
   const auto dateFormatStr = QString("d MMM yyyy");
   const auto leftDate = QDate::fromString(left.toString(), dateFormatStr);
   const auto rightDate = QDate::fromString(right.toString(), dateFormatStr);
   return leftDate < rightDate;
}

bool InvoiceSortProxyModel::isEqual(const double left, const double right)
{
   const double minDiff = 0.01;
   const double diff = left - right;
   return (diff < minDiff && diff > -minDiff);
}
