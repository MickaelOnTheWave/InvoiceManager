/*
 * InvoiceManager
 * Copyright (C) 2023 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "CompanyDetailsWidget.h"
#include "ui_CompanyDetailsWidget.h"

CompanyDetailsWidget::CompanyDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanyDetailsWidget)
{
    ui->setupUi(this);

    dataControls = {ui->nameEdit, ui->phoneEdit, ui->emailEdit, ui->addressLineEdit1,
                    ui->addressLineEdit2, ui->addressLineEdit3, ui->addressLineEdit4};
    connectTextEditChanges();
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
   for (auto lineEdit : dataControls)
      lineEdit->setText("");
}

void CompanyDetailsWidget::connectTextEditChanges()
{
   for (auto lineEdit : dataControls)
      connect(lineEdit, &QLineEdit::textChanged, this, &CompanyDetailsWidget::dataChanged);
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
