#include "MainPage.h"
#include "ui_MainPage.h"

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    connect(ui->newInvoiceButton, &QAbstractButton::clicked, this, &MainPage::createNewInvoice);
    connect(ui->moreButton, &QAbstractButton::clicked, this, &MainPage::goToMore);
    connect(ui->backButton, &QAbstractButton::clicked, this, &MainPage::closeDb);
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::setDisplayData(const QString &companyName, const QString& dbFile,
                              const int dbVersion)
{
    ui->companyNameLabel->setText(companyName);
    ui->dbStatusWidget->setData(dbFile, dbVersion);
}

void MainPage::connectViewsToModels(ClientModel *_clientModel, QAbstractItemModel *_stylesheetModel,
                                    InvoiceModel* _invoiceModel)
{
    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;
    invoiceModel = _invoiceModel;

    ui->clientsView->setModel(clientModel);
    ui->stylesheetsView->setModel(stylesheetModel);
    ui->invoiceContentView->setModel(invoiceModel);

    ui->clientsView->hideColumn(0);
    ui->clientsView->hideColumn(2);
    ui->clientsView->hideColumn(5);

    ui->stylesheetsView->hideColumn(0);

    ui->clientsView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->clientsView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->stylesheetsView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->stylesheetsView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->stylesheetsView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    ui->invoiceContentView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->invoiceContentView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}
