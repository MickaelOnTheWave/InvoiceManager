/*
 * InvoiceManager
 * Copyright (C) 2023 Guimarães Tecnologia Ltda
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

#include "InvoiceSortProxyModel.h"

#include <QDate>
#include <QLocale>

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
      QLocale locale(QLocale::English);
      const double leftDouble = locale.toDouble(leftData.toString());
      const double rightDouble = locale.toDouble(rightData.toString());
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
