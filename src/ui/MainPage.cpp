#include "MainPage.h"
#include "ui_MainPage.h"

#include "CompanyDetailsWidget.h"
#include "InvoiceViewDialog.h"
#include "NewDataDialog.h"

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

    connect(ui->titleBarWidget, &TitleBarWidget::settingsClicked, this, &MainPage::settingsClicked);
    connect(ui->titleBarWidget, &TitleBarWidget::aboutClicked, this, &MainPage::aboutClicked);

    setViewDefaults(ui->invoiceContentView);
    setViewDefaults(ui->clientsView);
    setViewDefaults(ui->templatesView);
    setViewDefaults(ui->stylesheetsView);
}

MainPage::~MainPage()
{
    delete ui;
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
    ui->invoiceContentView->setModel(invoiceModel);

    ui->clientsView->hideColumn(0);
    ui->clientsView->hideColumn(2);
    ui->clientsView->hideColumn(5);
    ui->invoiceContentView->hideColumn(0);

    initializeFileResourceView(ui->templatesView);
    initializeFileResourceView(ui->stylesheetsView);

    ui->clientsView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->clientsView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    ui->invoiceContentView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->invoiceContentView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void MainPage::onOpenInvoice(const QModelIndex &index)
{
    auto dialog = new InvoiceViewDialog(this);
    dialog->setData(createInvoiceTemplateData(index));
    dialog->show();
}

void MainPage::onOpenClient(const QModelIndex& index)
{
   auto companyWidget = new CompanyDetailsWidget();
   companyWidget->fill(createCompanyData(index));
   auto dialog = new NewDataDialog(companyWidget, this);
   dialog->setWindowTitle("Company Details");
   dialog->show();
}

void MainPage::initializeFileResourceView(QTableView *viewControl)
{
    viewControl->hideColumn(0);
    viewControl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    viewControl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    viewControl->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void MainPage::setViewDefaults(QTableView *view)
{
    view->verticalHeader()->hide();
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
}

InvoiceTemplateData MainPage::createInvoiceTemplateData(const QModelIndex& index)
{
   const int invoiceId = invoiceModel->data(invoiceModel->index(index.row(), 0)).toInt();
   return controller->getInvoiceTemplateData(invoiceId);
}

CompanyData MainPage::createCompanyData(const QModelIndex& index)
{
   return clientModel->getDataAtRow(index.row());
}

