#include "InvoiceViewDialog.h"
#include "ui_InvoiceViewDialog.h"

InvoiceViewDialog::InvoiceViewDialog(const int _invoiceId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvoiceViewDialog),
    invoiceId(_invoiceId)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QAbstractButton::clicked, this, &QDialog::close);
}

InvoiceViewDialog::~InvoiceViewDialog()
{
    delete ui;
}
