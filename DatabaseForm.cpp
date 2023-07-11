#include "DatabaseForm.h"
#include "ui_DatabaseForm.h"

DatabaseForm::DatabaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseForm)
{
    ui->setupUi(this);
    connect(ui->newInvoiceButton, &QAbstractButton::clicked, this, &DatabaseForm::createNewInvoice);
    connect(ui->moreButton, &QAbstractButton::clicked, this, &DatabaseForm::goToMore);

    connect(ui->dbStatusWidget, &DbStatusForm::closeAndSave, this, &DatabaseForm::closeAndSave);
    connect(ui->dbStatusWidget, &DbStatusForm::closeAndDiscard, this, &DatabaseForm::closeAndDiscard);
}

DatabaseForm::~DatabaseForm()
{
    delete ui;
}

void DatabaseForm::setCompanyName(const QString &value)
{
    ui->companyNameLabel->setText(value);
}

void DatabaseForm::connectViewsToModels(QAbstractItemModel *_clientModel, QAbstractItemModel *_stylesheetModel)
{
    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;

    ui->clientsView->setModel(clientModel);
    ui->stylesheetsView->setModel(stylesheetModel);
}
