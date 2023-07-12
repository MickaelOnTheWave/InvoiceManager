#include "MorePage.h"
#include "ui_MorePage.h"

#include <QMessageBox>

#include "Company.h"
#include "CompanyDetailsWidget.h"
#include "StylesheetDetailsWidget.h"
#include "NewDataDialog.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);

    connect(ui->clientsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddClient);
    connect(ui->stylesheetsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddStylesheet);
    connect(ui->backButton, &QAbstractButton::clicked, this, &MorePage::back);
}

MorePage::~MorePage()
{
    delete ui;
}

void MorePage::connectViewsToModels(ClientModel *_clientModel,
                                    StylesheetModel *_stylesheetModel)
{
    ui->clientsWidget->setModel(_clientModel);
    ui->stylesheetsWidget->setModel(_stylesheetModel);

    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;
}

void MorePage::onAddClient()
{
    auto contentWidget = new CompanyDetailsWidget();
    addDataToModel(contentWidget, [this, contentWidget] () {
        return clientModel->insertAtEnd(contentWidget->getData());
    });
}

void MorePage::onAddStylesheet()
{
    auto contentWidget = new StylesheetDetailsWidget();
    addDataToModel(contentWidget, [this, contentWidget] () {
        return stylesheetModel->insertAtEnd(contentWidget->getPath());
    });
}

bool MorePage::insertInStylesheetModel()
{
    const int rowIndex = stylesheetModel->rowCount();
    const bool ok = stylesheetModel->insertRow(rowIndex);
    if (!ok)
        return false;

    stylesheetModel->setData(stylesheetModel->index(rowIndex, 0), "stylesheet.css");

    return false;
}

void MorePage::addDataToModel(QWidget *dataWidget, std::function<bool ()> insertDataFunc)
{
    NewDataDialog dialog(dataWidget, this);
    const int result = dialog.exec();
    if (result == QDialog::Accepted)
    {
        const bool ok = insertDataFunc();
        if (!ok)
            QMessageBox::warning(&dialog, "Error", "Could not insert data into model");
    }
}
