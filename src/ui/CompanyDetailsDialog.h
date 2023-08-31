#ifndef COMPANYDETAILSDIALOG_H
#define COMPANYDETAILSDIALOG_H

#include <QDialog>

#include "CompanyData.h"
#include "InvoiceDbController.h"

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
