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

#include "MorePage.h"
#include "ui_MorePage.h"

#include <QMessageBox>

#include "CompanyData.h"
#include "CompanyDetailsWidget.h"
#include "ConfirmRemoveDialog.h"
#include "GuiUtils.h"
#include "FileResourceAddWidget.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);

    connect(ui->clientsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddClient);
    connect(ui->clientsWidget, &DataHandlerWidget::removeClicked, this, &MorePage::onRemoveClient);
    connect(ui->templatesWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddTemplate);
    connect(ui->templatesWidget, &DataHandlerWidget::removeClicked, this, &MorePage::onRemoveTemplate);
    connect(ui->stylesheetsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddStylesheet);
    connect(ui->stylesheetsWidget, &DataHandlerWidget::removeClicked, this, &MorePage::onRemoveStylesheet);
    connect(ui->backButton, &QAbstractButton::clicked, this, &MorePage::back);

    connect(ui->titleBarWidget, &TitleBarWidget::settingsClicked, this, &MorePage::settingsClicked);
    connect(ui->titleBarWidget, &TitleBarWidget::aboutClicked, this, &MorePage::aboutClicked);
}

MorePage::~MorePage()
{
   delete ui;
}

void MorePage::setCompanyTitle(const QString& title)
{
   ui->titleBarWidget->setTitle(title);
}

void MorePage::setController(InvoiceDbController* _controller)
{
   controller = _controller;
}

void MorePage::connectViewsToModels(ClientModel *_clientModel,
                                    FileResourceModel *_templateModel,
                                    FileResourceModel *_stylesheetModel)
{
    ui->clientsWidget->setModel(_clientModel);
    ui->templatesWidget->setModel(_templateModel);
    ui->stylesheetsWidget->setModel(_stylesheetModel);

    ui->clientsWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch});
    ui->templatesWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch});
    ui->stylesheetsWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch});

    clientModel = _clientModel;
    templateModel = _templateModel;
    stylesheetModel = _stylesheetModel;

    ui->clientsWidget->hideColumns({0, 2, 5});
    ui->templatesWidget->hideColumns({2, 3});
    ui->stylesheetsWidget->hideColumns({2, 3});
}

void MorePage::onAddClient()
{
    auto contentWidget = new CompanyDetailsWidget();
    GuiUtils::addDataToModel(contentWidget, tr("New Client"), [this, contentWidget] () {
        return clientModel->insertAtEnd(contentWidget->getData());
    });
}

void MorePage::onAddTemplate()
{
    GuiUtils::OnAddTemplate(templateModel);
}

void MorePage::onAddStylesheet()
{
   GuiUtils::OnAddStylesheet(stylesheetModel);
}

void MorePage::onRemoveClient(const QModelIndex index)
{
   auto deleter = [this] (const QModelIndex& index) {
      clientModel->remove(index);
   };
   onRemoveFromModel(index, clientModel, "clientId", deleter);
}

void MorePage::onRemoveTemplate(const QModelIndex index)
{
   onRemoveFileResource(index, templateModel, "templateId");
}

void MorePage::onRemoveStylesheet(const QModelIndex index)
{
   onRemoveFileResource(index, stylesheetModel, "stylesheetId");
}

bool MorePage::canRemoveFileResource(const int id, const QString& dbField) const
{
   const int usingCount = controller->getInvoiceCountUsingFile(id, dbField);
   if (usingCount > 0)
   {
      const QString message = tr("There are %1 invoices using this resource. If you wish to remove it, you need to first"
                                 " remove the invoices that are using it.").arg(usingCount);
      QMessageBox::warning(nullptr, "Error", message);
      return false;
   }
   return true;
}

void MorePage::onRemoveFileResource(const QModelIndex index, FileResourceModel* model,
                                    const QString& dbField)
{
   auto deleter = [model](const QModelIndex& index) {
      model->remove(index);
   };
   onRemoveFromModel(index, model, dbField, deleter);
}

void MorePage::onRemoveFromModel(const QModelIndex& index, QAbstractItemModel* model, const QString& dbField,
                                 std::function<void (const QModelIndex&)> deleteFunc)
{
   const int selectedId = model->data(model->index(index.row(), 0)).toInt();
   if (!canRemoveFileResource(selectedId, dbField))
      return;

   const bool removed = ConfirmRemoveDialog::ask("Are you sure?", "user/removefileconfirmation");
   if (removed)
      deleteFunc(index);
}
