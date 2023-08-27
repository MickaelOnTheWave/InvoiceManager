#include "InvoiceViewDialog.h"
#include "ui_InvoiceViewDialog.h"

#include <QMessageBox>

InvoiceViewDialog::InvoiceViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvoiceViewDialog)
{
    ui->setupUi(this);
    connect(ui->deleteButton, &QAbstractButton::clicked, this, &InvoiceViewDialog::onDeleteClicked);
    connect(ui->closeButton, &QAbstractButton::clicked, this, &QDialog::close);
}

InvoiceViewDialog::~InvoiceViewDialog()
{
   delete ui;
}

void InvoiceViewDialog::setData(const InvoiceTemplateData& data)
{
   invoiceData = data;
   ui->viewWidget->show(data);
}

void InvoiceViewDialog::onDeleteClicked()
{
   const QString message = tr("This invoice will be permanently removed from the database."
                              "Are you sure you want to proceed ?");
   const auto selectedButton = QMessageBox::question(nullptr, "Removal", message);
   if (selectedButton == QMessageBox::Yes)
   {
      emit deleteConfirmed(invoiceData.id);
      close();
   }
}

