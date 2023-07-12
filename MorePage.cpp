#include "MorePage.h"
#include "ui_MorePage.h"

#include <QMessageBox>

#include "Company.h"
#include "CompanyDetailsWidget.h"
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
                                    QAbstractItemModel *_stylesheetModel)
{
    ui->clientsWidget->setModel(_clientModel);
    ui->stylesheetsWidget->setModel(_stylesheetModel);

    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;
}

void MorePage::onAddClient()
{
    auto contentWidget = new CompanyDetailsWidget();
    NewDataDialog dialog(contentWidget, this);
    const int result = dialog.exec();
    if (result == QDialog::Accepted)
    {
        const bool ok = clientModel->insertAtEnd(contentWidget->getData());
        if (!ok)
            QMessageBox::warning(&dialog, "Error", "Could not insert data into model");
    }
}

void MorePage::onAddStylesheet()
{
    // TODO : Implement real one, with a dialog and selecting stylesheet etc...

    insertInStylesheetModel();

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
