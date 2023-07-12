#include "MainPage.h"
#include "ui_MainPage.h"

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    connect(ui->newInvoiceButton, &QAbstractButton::clicked, this, &MainPage::createNewInvoice);
    connect(ui->moreButton, &QAbstractButton::clicked, this, &MainPage::goToMore);

    connect(ui->dbStatusWidget, &DbStatusForm::closeAndSave, this, &MainPage::closeAndSave);
    connect(ui->dbStatusWidget, &DbStatusForm::closeAndDiscard, this, &MainPage::closeAndDiscard);
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::setCompanyName(const QString &value)
{
    ui->companyNameLabel->setText(value);
}

void MainPage::connectViewsToModels(ClientModel *_clientModel, QAbstractItemModel *_stylesheetModel)
{
    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;

    ui->clientsView->setModel(clientModel);
    ui->stylesheetsView->setModel(stylesheetModel);

    ui->clientsView->hideColumn(0);
    ui->clientsView->resizeColumnsToContents();

    ui->stylesheetsView->hideColumn(0);
    ui->stylesheetsView->resizeColumnsToContents();
}
