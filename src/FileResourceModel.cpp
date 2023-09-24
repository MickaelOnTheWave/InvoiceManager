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

#include "FileResourceModel.h"

FileResourceModel::FileResourceModel(const QString &tableName, QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    setTable(tableName);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    setHeaderData(0, Qt::Horizontal, tr("Id"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("File"));
}

int FileResourceModel::getId(const int i) const
{
    return data(index(i, 0)).toInt();
}

bool FileResourceModel::insertAtEnd(const QString &name,
                                  const QString &path)
{
    const int rowIndex = rowCount();
    const bool ok = insertRow(rowIndex);
    if (!ok)
        return false;

    setData(index(rowIndex, 1), name);
    setData(index(rowIndex, 2), path);
    return submitAll();
}

bool FileResourceModel::remove(const QModelIndex& index)
{
   bool ok = removeRow(index.row());
   if (!ok)
      return false;

   return submitAll();
}
