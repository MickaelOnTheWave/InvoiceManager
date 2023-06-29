#ifndef COMPANYDETAILSWIDGET_H
#define COMPANYDETAILSWIDGET_H

#include <QWidget>

#include "Company.h"

namespace Ui {
class CompanyDetailsWidget;
}

class CompanyDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompanyDetailsWidget(QWidget *parent = nullptr);
    ~CompanyDetailsWidget();

    CompanyData getData() const;

private:
    QString buildAddress() const;
    static void addAddressLine(QString& finalAddress, const QString& addressLine);

    Ui::CompanyDetailsWidget *ui;
};

#endif // COMPANYDETAILSWIDGET_H
