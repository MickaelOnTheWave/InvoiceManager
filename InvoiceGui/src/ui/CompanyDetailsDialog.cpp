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

#include "CompanyDetailsDialog.h"
#include "ui_CompanyDetailsDialog.h"

CompanyDetailsDialog::CompanyDetailsDialog(InvoiceDbController* _controller, QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CompanyDetailsDialog),
   controller(_controller)
{
   ui->setupUi(this);
   ui->updateButton->hide();

   connect(ui->companyWidget, &CompanyDetailsWidget::dataChanged, this, &CompanyDetailsDialog::onDataChanged);
   connect(ui->updateButton, &QAbstractButton::clicked, this, &CompanyDetailsDialog::onUpdateCompany);
   connect(ui->closeButton, &QAbstractButton::clicked, this, &QDialog::close);
}

CompanyDetailsDialog::~CompanyDetailsDialog()
{
   delete ui;
}

void CompanyDetailsDialog::setData(const CompanyData& _data, const int _id)
{
   data = _data;
   companyId = _id;
   ui->companyWidget->fill(data);
}

void CompanyDetailsDialog::onDataChanged()
{
   const bool isDataSameAsInitial = (data == ui->companyWidget->getData());
   ui->updateButton->setVisible(!isDataSameAsInitial);
}

void CompanyDetailsDialog::onUpdateCompany()
{
   controller->writeUpdatedCompany(ui->companyWidget->getData(), companyId);
   emit createdUpdatedCompany();
   close();
}
