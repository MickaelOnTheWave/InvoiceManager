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

#ifndef COMPANYDETAILSDIALOG_H
#define COMPANYDETAILSDIALOG_H

#include <QDialog>

#include "InvoiceModule/include/CompanyData.h"
#include "InvoiceModule/include/InvoiceDbController.h"

namespace Ui {
   class CompanyDetailsDialog;
}

class CompanyDetailsDialog : public QDialog
{
   Q_OBJECT

public:
   explicit CompanyDetailsDialog(InvoiceDbController* _controller,
                                 QWidget *parent = nullptr);
   ~CompanyDetailsDialog();

   void setData(const CompanyData& _data, const int _id);

signals:
   void createdUpdatedCompany();

private slots:
   void onDataChanged();
   void onUpdateCompany();

private:
   Ui::CompanyDetailsDialog *ui;

   InvoiceDbController* controller;
   CompanyData data;
   int companyId;
};

#endif // COMPANYDETAILSDIALOG_H
