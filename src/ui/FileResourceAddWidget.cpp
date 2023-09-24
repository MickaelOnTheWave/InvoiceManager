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

#include "FileResourceAddWidget.h"
#include "ui_FileResourceAddWidget.h"

#include <QFileDialog>

FileResourceAddWidget::FileResourceAddWidget(const QString &fileFilter, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileResourceAddWidget),
    typeDescription(fileFilter)
{
    ui->setupUi(this);

    connect(ui->pathButton, &QAbstractButton::clicked, this, &FileResourceAddWidget::onBrowseClick);
}

FileResourceAddWidget::~FileResourceAddWidget()
{
    delete ui;
}

QString FileResourceAddWidget::getName() const
{
    return ui->nameEdit->text();
}

QString FileResourceAddWidget::getPath() const
{
    return ui->pathEdit->text();
}

void FileResourceAddWidget::onBrowseClick()
{
    const QString title = tr("Select a file");
    QString filePath = QFileDialog::getOpenFileName(this, title, "", typeDescription);
    if (!filePath.isEmpty())
    {
        ui->pathEdit->setText(filePath);
    }
}
