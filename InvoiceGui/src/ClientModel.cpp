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

#include "ClientModel.h"

#include <QSqlQuery>
#include "InvoiceDbController.h"

#include <QSqlField>
#include <QSqlRecord>

ClientModel::ClientModel(InvoiceDbController* _controller, const bool _separateChildCompanies,
                         QObject *parent)
    : QSqlQueryModel(parent),
   controller(_controller),
  separateChildCompanies(_separateChildCompanies)
{
    refreshModel();
}

void ClientModel::setSeparateChildCompanies(const bool separate)
{
   separateChildCompanies = separate;
}

int ClientModel::getId(const int i) const
{
    return data(index(i, 0)).toInt();
}

CompanyData ClientModel::getDataAtRow(const int i) const
{
    CompanyData companyData;
    companyData.name = data(index(i, 1)).toString();
    companyData.address = data(index(i, 2)).toString();
    companyData.email = data(index(i, 3)).toString();
    companyData.phoneNumber = data(index(i, 4)).toString();
    return companyData;
}

bool ClientModel::insertAtEnd(const CompanyData &data)
{
    const int rowIndex = rowCount();

    QSqlQuery query = InvoiceDbController::createWriteCompanyQuery(data, true);

    const bool result = query.exec();
    if (!result)
        return false;

    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    endInsertRows();

    refreshModel();
    return result;
}

bool ClientModel::remove(const QModelIndex& i)
{
   const int id = data(index(i.row(), 0)).toInt();
   const int parentCompanyId = controller->getParentCompanyId(id);
   if (parentCompanyId > -1)
   {
      const bool result = controller->updateCompanyParenting(parentCompanyId, -1);
      if (!result)
         return false;
   }

   QString queryStr = "DELETE FROM company WHERE id = %1";
   QSqlQuery query;
   const bool result = query.exec(queryStr.arg(id));
   if (result)
      refreshModel();
   return result;
}

void ClientModel::refreshModel()
{
   QString queryStr = "SELECT * FROM company WHERE isClient = TRUE";
   if (!separateChildCompanies)
      queryStr += " AND idChild = -1";

   setQuery(queryStr);
   setHeaderData(0, Qt::Horizontal, tr("Id"));
   setHeaderData(1, Qt::Horizontal, tr("Name"));
   setHeaderData(2, Qt::Horizontal, tr("Address"));
   setHeaderData(3, Qt::Horizontal, tr("Email"));
   setHeaderData(4, Qt::Horizontal, tr("Phone number"));
}
