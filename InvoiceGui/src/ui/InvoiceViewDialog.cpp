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

#include "InvoiceViewDialog.h"
#include "ui_InvoiceViewDialog.h"

#include <QFile>
#include <QMessageBox>

InvoiceViewDialog::InvoiceViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvoiceViewDialog)
{
    ui->setupUi(this);
    connect(ui->deleteButton, &QAbstractButton::clicked, this, &InvoiceViewDialog::onDeleteClicked);
    connect(ui->closeButton, &QAbstractButton::clicked, this, &QDialog::close);
    connect(ui->createPdfButton, &QAbstractButton::clicked, this, &InvoiceViewDialog::onCreatePdf);
}

InvoiceViewDialog::~InvoiceViewDialog()
{
   delete ui;
}

void InvoiceViewDialog::setData(const InvoiceUserData& data)
{
   invoiceDocument.setData(data);
   ui->viewWidget->show(invoiceDocument.CreateHtmlContent(), data.stylesheetPath);
}

void InvoiceViewDialog::onDeleteClicked()
{
   const QString message = tr("This invoice will be permanently removed from the database."
                              "Are you sure you want to proceed ?");
   const auto selectedButton = QMessageBox::question(nullptr, "Removal", message);
   if (selectedButton == QMessageBox::Yes)
   {
      emit deleteConfirmed(invoiceDocument.GetInvoiceId());
      close();
   }
}

void InvoiceViewDialog::onCreatePdf()
{
   invoiceDocument.CreatePdfFile("/home/mickael/2023-09 Consulting Invoice.pdf");
}

