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

#include "MainPage.h"
#include "ui_MainPage.h"


#include "CompanyDetailsDialog.h"
#include "FileResourceDialog.h"
#include "InvoiceSortProxyModel.h"
#include "InvoiceViewDialog.h"

// TODO add checkbox to display (or not) old parent companies

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    connect(ui->newInvoiceFromEmptyButton, &QAbstractButton::clicked, this, &MainPage::createNewInvoice);
    connect(ui->newInvoiceFromLastButton, &QAbstractButton::clicked, this, &MainPage::createNewInvoiceFromLast);
    connect(ui->moreButton, &QAbstractButton::clicked, this, &MainPage::goToMore);
    connect(ui->backButton, &QAbstractButton::clicked, this, &MainPage::closeDb);

    connect(ui->invoiceContentView, &QTableView::doubleClicked, this, &MainPage::onOpenInvoice);
    connect(ui->clientsView, &QTableView::doubleClicked, this, &MainPage::onOpenClient);
    connect(ui->templatesView, &QTableView::doubleClicked, this, &MainPage::onOpenTemplate);
    connect(ui->stylesheetsView, &QTableView::doubleClicked, this, &MainPage::onOpenStylesheet);

    connect(ui->titleBarWidget, &TitleBarWidget::settingsClicked, this, &MainPage::settingsClicked);
    connect(ui->titleBarWidget, &TitleBarWidget::aboutClicked, this, &MainPage::aboutClicked);

    setViewDefaults(ui->invoiceContentView);
    setViewDefaults(ui->clientsView);
    setViewDefaults(ui->templatesView);
    setViewDefaults(ui->stylesheetsView);

    ui->invoiceContentView->horizontalHeader()->setSortIndicatorShown(true);
}

MainPage::~MainPage()
{
   delete ui;
}

void MainPage::update()
{
   const bool enable = (controller->getLastInvoiceId() > -1);
   ui->newInvoiceFromLastButton->setVisible(enable);
}

void MainPage::setDisplayData(const QString &companyName, const QString& dbFile,
                              const int dbVersion)
{
   ui->titleBarWidget->setTitle(companyName);
   ui->dbStatusWidget->setData(dbFile, dbVersion);
}

void MainPage::setController(InvoiceDbController* _controller)
{
   controller = _controller;
}

void MainPage::connectViewsToModels(ClientModel *_clientModel, QAbstractItemModel *_templateModel,
                                    QAbstractItemModel *_stylesheetModel, InvoiceModel* _invoiceModel)
{
   clientModel = _clientModel;
   templateModel = _templateModel;
   stylesheetModel = _stylesheetModel;
   invoiceModel = _invoiceModel;

   ui->clientsView->setModel(clientModel);
   ui->templatesView->setModel(templateModel);
   ui->stylesheetsView->setModel(stylesheetModel);

   auto proxyModel = new InvoiceSortProxyModel(this);
   proxyModel->setSourceModel(invoiceModel);
   ui->invoiceContentView->setModel(proxyModel);
   ui->invoiceContentView->setSortingEnabled(true);

   ui->clientsView->hideColumn(0); // Id
   ui->clientsView->hideColumn(2); // Address
   ui->clientsView->hideColumn(5); // isClient
   ui->clientsView->hideColumn(6); // idChild
   ui->invoiceContentView->hideColumn(0);

   initializeFileResourceView(ui->templatesView);
   initializeFileResourceView(ui->stylesheetsView);

   ui->clientsView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
   ui->clientsView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

   ui->invoiceContentView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
   ui->invoiceContentView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

   auto header = ui->invoiceContentView->horizontalHeader();
   connect(header, &QHeaderView::sectionClicked, this, &MainPage::onInvoiceHeaderClicked);
}

void MainPage::onOpenInvoice(const QModelIndex &index)
{
   auto proxyModel = dynamic_cast<InvoiceSortProxyModel*>(ui->invoiceContentView->model());
   const QModelIndex originalIndex = proxyModel->mapToSource(index);
   auto dialog = new InvoiceViewDialog(this);
   dialog->setData(createInvoiceTemplateData(originalIndex));
   dialog->show();
   connect(dialog, &InvoiceViewDialog::deleteConfirmed, this, &MainPage::onRemoveInvoice);
}

void MainPage::onOpenClient(const QModelIndex& index)
{
   auto dialog = new CompanyDetailsDialog(controller, this);
   const int companyId = clientModel->getId(index.row());
   dialog->setData(createCompanyData(index), companyId);
   dialog->show();
   connect(dialog, &CompanyDetailsDialog::createdUpdatedCompany, this, &MainPage::onCompanyUpdateCreated);
}

void MainPage::onOpenTemplate(const QModelIndex& index)
{
   auto dialog = new FileResourceDialog(templateModel, index, this);
   dialog->show();
}

void MainPage::onOpenStylesheet(const QModelIndex& index)
{
   auto dialog = new FileResourceDialog(stylesheetModel, index, this);
   dialog->show();
}

void MainPage::onRemoveInvoice(const int id)
{
   controller->removeInvoice(id);
   invoiceModel->refresh();
}

void MainPage::onCompanyUpdateCreated()
{
   clientModel->refreshModel();
}

void MainPage::onInvoiceHeaderClicked(int logicalIndex)
{
   auto view = ui->invoiceContentView;
   view->sortByColumn(logicalIndex, view->horizontalHeader()->sortIndicatorOrder());
}

void MainPage::initializeFileResourceView(QTableView *viewControl)
{
    viewControl->hideColumn(0);
    viewControl->hideColumn(2);
    viewControl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    viewControl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    viewControl->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void MainPage::setViewDefaults(QTableView *view)
{
    view->verticalHeader()->hide();
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

InvoiceUserData MainPage::createInvoiceTemplateData(const QModelIndex& index)
{
   const int invoiceId = invoiceModel->data(invoiceModel->index(index.row(), 0)).toInt();
   return controller->getInvoiceUserData(invoiceId);
}

CompanyData MainPage::createCompanyData(const QModelIndex& index)
{
   return clientModel->getDataAtRow(index.row());
}

