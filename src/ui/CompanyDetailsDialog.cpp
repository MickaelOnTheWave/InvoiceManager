#include "CompanyDetailsDialog.h"
#include "ui_CompanyDetailsDialog.h"

CompanyDetailsDialog::CompanyDetailsDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CompanyDetailsDialog)
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

void CompanyDetailsDialog::setData(const CompanyData& _data)
{
   data = _data;
   ui->companyWidget->fill(data);
}

void CompanyDetailsDialog::onDataChanged()
{
   const bool isDataSameAsInitial = (data == ui->companyWidget->getData());
   ui->updateButton->setVisible(!isDataSameAsInitial);
}

void CompanyDetailsDialog::onUpdateCompany()
{
   // TODO :
   // - Add controller in this class
   // - Call controller to update company
   // - Implement controller update :
   //   - Update parent company child prop ?
   //   - Insert new company entry
}
