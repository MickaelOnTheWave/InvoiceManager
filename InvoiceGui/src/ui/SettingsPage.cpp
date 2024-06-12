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

#include "SettingsPage.h"
#include "ui_SettingsPage.h"

#include <QDialogButtonBox>
#include <QSettings>

SettingsPage::SettingsPage(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::SettingsPage)
{
   ui->setupUi(this);

   connect(ui->resetButton, &QPushButton::clicked, this, &SettingsPage::onReset);
   connect(ui->quitBox, &QDialogButtonBox::accepted, this, &SettingsPage::onSave);
   connect(ui->quitBox->button(QDialogButtonBox::Discard), &QAbstractButton::clicked,
           this, &SettingsPage::done);

   loadSettings();
}

SettingsPage::~SettingsPage()
{
   delete ui;
}

void SettingsPage::refresh()
{
   loadSettings();
}

void SettingsPage::onReset()
{
   ui->removeInvoiceBox->setChecked(true);
   ui->removeFileBox->setChecked(true);
}

void SettingsPage::onSave()
{
   saveSettings();
   emit done();
}

void SettingsPage::loadSettings()
{
   QSettings settings;

   const bool invoiceConfirm = settings.value("user/removeinvoiceconfirmation", true).toBool();
   ui->removeInvoiceBox->setChecked(invoiceConfirm);

   const bool fileConfirm = settings.value("user/removefileconfirmation", true).toBool();
   ui->removeFileBox->setChecked(fileConfirm);
}

void SettingsPage::saveSettings()
{
   QSettings settings;
   settings.setValue("user/removeinvoiceconfirmation", ui->removeInvoiceBox->isChecked());
   settings.setValue("user/removefileconfirmation", ui->removeFileBox->isChecked());
}
