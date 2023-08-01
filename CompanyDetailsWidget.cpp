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

void CompanyDetailsWidget::fill(const CompanyData &data)
{
    ui->nameEdit->setText(data.name);
    ui->emailEdit->setText(data.email);
    ui->phoneEdit->setText(data.phoneNumber);

    const QStringList explodedAddress = data.address.split("\n");
    ui->addressLineEdit1->setText(getExplodedValue(explodedAddress, 0));
    ui->addressLineEdit2->setText(getExplodedValue(explodedAddress, 1));
    ui->addressLineEdit3->setText(getExplodedValue(explodedAddress, 2));
    ui->addressLineEdit4->setText(getExplodedValue(explodedAddress, 3));
}

void CompanyDetailsWidget::reset()
{
    ui->nameEdit->setText("");
    ui->emailEdit->setText("");
    ui->phoneEdit->setText("");

    ui->addressLineEdit1->setText("");
    ui->addressLineEdit2->setText("");
    ui->addressLineEdit3->setText("");
    ui->addressLineEdit4->setText("");
}

QString CompanyDetailsWidget::buildAddress() const
{
    QString address = ui->addressLineEdit1->text();
    addAddressLine(address, ui->addressLineEdit2->text());
    addAddressLine(address, ui->addressLineEdit3->text());
    addAddressLine(address, ui->addressLineEdit4->text());
    return address;
}

void CompanyDetailsWidget::addAddressLine(QString &finalAddress, const QString &addressLine)
{
    if (!addressLine.isEmpty())
        finalAddress += "\n" + addressLine;
}

QString CompanyDetailsWidget::getExplodedValue(const QStringList &exploded, const int index)
{
    return (exploded.size() > index) ? exploded[index] : "";
}
