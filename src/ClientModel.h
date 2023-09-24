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

#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QSqlQueryModel>

#include "InvoiceModule/include/CompanyData.h"
#include "InvoiceModule/include/InvoiceDbController.h"

class ClientModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ClientModel(InvoiceDbController* _controller,
                         QObject *parent = nullptr);

    int getId(const int i) const;
    CompanyData getDataAtRow(const int i) const;

    bool insertAtEnd(const CompanyData& data);
    bool remove(const QModelIndex& i);

    void refreshModel();

private:
    InvoiceDbController* controller;
};

#endif // CLIENTMODEL_H
