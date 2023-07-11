#include "MorePage.h"
#include "ui_MorePage.h"

#include <QMessageBox>

#include "CompanyDetailsWidget.h"
#include "NewDataDialog.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);

    connect(ui->clientsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddClient);
    connect(ui->backButton, &QAbstractButton::clicked, this, &MorePage::back);
}

MorePage::~MorePage()
{
    delete ui;
}

void MorePage::connectViewsToModels(QAbstractItemModel *_clientModel,
                                    QAbstractItemModel *_stylesheetModel)
{
    ui->clientsWidget->setModel(_clientModel);
    ui->stylesheetsView->setModel(_stylesheetModel);

    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;

    //    ui->clientsView->show();
}

void MorePage::onAddClient()
{
    auto contentWidget = new CompanyDetailsWidget();
    NewDataDialog dialog(contentWidget, this);
    const int result = dialog.exec();
    if (result == QDialog::Accepted)
    {
        const bool ok = insertInClientModel(contentWidget->getData());
        if (!ok)
            QMessageBox::warning(&dialog, "Error", "Could not insert data into model");
    }
}

bool MorePage::insertInClientModel(const CompanyData &data)
{
    const int rowIndex = clientModel->rowCount();
    const bool ok = clientModel->insertRow(rowIndex);
    if (!ok)
        return false;

    clientModel->setData(clientModel->index(rowIndex, 1), data.name);
    clientModel->setData(clientModel->index(rowIndex, 2), data.address);
    return true;
}
