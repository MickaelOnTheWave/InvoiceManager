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

#include "CreatePage.h"
#include "ui_CreatePage.h"

#include <QFileDialog>
#include "GuiUtils.h"

CreatePage::CreatePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreatePage)
{
    ui->setupUi(this);

    connect(ui->templatesDataWidget, &DataHandlerWidget::addClicked, this, &CreatePage::onAddTemplate);
    connect(ui->stylesheetsDataWidget, &DataHandlerWidget::addClicked, this, &CreatePage::onAddStylesheet);

    connect(ui->confirmButton, &QAbstractButton::clicked, this, &CreatePage::confirm);
    connect(ui->cancelButton, &QAbstractButton::clicked, this, &CreatePage::cancel);
}

CreatePage::~CreatePage()
{
    delete ui;
}

void CreatePage::reset()
{
    ui->addressWidget->reset();
}

CompanyData CreatePage::getCompanyData() const
{
    return ui->addressWidget->getData();
}

void CreatePage::setModel(FileResourceModel *_templateModel, FileResourceModel *_stylesheetModel)
{
    templateModel = _templateModel;
    ui->templatesDataWidget->setModel(_templateModel);
    ui->templatesDataWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch});

    stylesheetModel = _stylesheetModel;
    ui->stylesheetsDataWidget->setModel(_stylesheetModel);
    ui->stylesheetsDataWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch});
}

void CreatePage::onAddStylesheet()
{
    GuiUtils::OnAddStylesheet(stylesheetModel);
}

void CreatePage::onAddTemplate()
{
    GuiUtils::OnAddTemplate(templateModel);
}
