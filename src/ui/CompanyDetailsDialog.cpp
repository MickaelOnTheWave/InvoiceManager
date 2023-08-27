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
