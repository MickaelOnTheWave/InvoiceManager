#include "InvoiceViewDialog.h"
#include "ui_InvoiceViewDialog.h"

InvoiceViewDialog::InvoiceViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvoiceViewDialog)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QAbstractButton::clicked, this, &QDialog::close);
}

InvoiceViewDialog::~InvoiceViewDialog()
{
   delete ui;
}

void InvoiceViewDialog::setData(const InvoiceTemplateData& data)
{
   ui->viewWidget->show(data);
}

