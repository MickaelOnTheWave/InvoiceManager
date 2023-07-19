#include "MorePage.h"
#include "ui_MorePage.h"

#include <QMessageBox>

#include "Company.h"
#include "CompanyDetailsWidget.h"
#include "GuiUtils.h"
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

    ui->clientsWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch});
    ui->stylesheetsWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch});

    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;
}

void MorePage::onAddClient()
{
    auto contentWidget = new CompanyDetailsWidget();
    GuiUtils::addDataToModel(contentWidget, [this, contentWidget] () {
        return clientModel->insertAtEnd(contentWidget->getData());
    });
}

void MorePage::onAddStylesheet()
{
    GuiUtils::OnAddStylesheet(stylesheetModel);
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
