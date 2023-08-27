#ifndef COMPANYDETAILSDIALOG_H
#define COMPANYDETAILSDIALOG_H

#include <QDialog>

#include "CompanyData.h"

namespace Ui {
   class CompanyDetailsDialog;
}

class CompanyDetailsDialog : public QDialog
{
   Q_OBJECT

public:
   explicit CompanyDetailsDialog(QWidget *parent = nullptr);
   ~CompanyDetailsDialog();

   void setData(const CompanyData& _data);

private slots:
   void onDataChanged();
   void onUpdateCompany();

private:
   Ui::CompanyDetailsDialog *ui;

   CompanyData data;
};

#endif // COMPANYDETAILSDIALOG_H
