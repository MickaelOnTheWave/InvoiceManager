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

#include "AboutPage.h"
#include "ui_AboutPage.h"

AboutPage::AboutPage(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::AboutPage)
{
   ui->setupUi(this);

   connect(ui->backButton, &QAbstractButton::clicked, this, &AboutPage::backClicked);
}

AboutPage::~AboutPage()
{
   delete ui;
}

void AboutPage::setVersions(const QString& appVersion, const int dbVersion)
{
   ui->appVersionLabel->setText(appVersion);
   ui->dbVersionLabel->setText(QString::number(dbVersion));
}
