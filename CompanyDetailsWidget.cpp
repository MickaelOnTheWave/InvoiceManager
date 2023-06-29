#include "CompanyDetailsWidget.h"
#include "ui_CompanyDetailsWidget.h"

CompanyDetailsWidget::CompanyDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanyDetailsWidget)
{
    ui->setupUi(this);
}

CompanyDetailsWidget::~CompanyDetailsWidget()
{
    delete ui;
}

CompanyData CompanyDetailsWidget::getData() const
{
    CompanyData data;
    data.name = ui->nameEdit->text();
    data.email = ui->emailEdit->text();
    data.phoneNumber = ui->phoneEdit->text();
    data.address = buildAddress();
    return data;
}

QString CompanyDetailsWidget::buildAddress() const
{
    QString address = ui->addressLineEdit1->text();
    addAddressLine(address, ui->addressLineEdit2->text());
    addAddressLine(address, ui->addressLineEdit3->text());
    return address;
}

void CompanyDetailsWidget::addAddressLine(QString &finalAddress, const QString &addressLine)
{
    if (!addressLine.isEmpty())
        finalAddress += "\n" + addressLine;
}
