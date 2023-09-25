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

#ifndef INVOICEMODEL_H
#define INVOICEMODEL_H

#include <QSqlQueryModel>

class InvoiceModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit InvoiceModel(QObject *parent = nullptr);

   QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const override;

    void refresh();
};

#endif // INVOICEMODEL_H
