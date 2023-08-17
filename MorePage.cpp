#include "MorePage.h"
#include "ui_MorePage.h"

#include <QMessageBox>

#include "CompanyData.h"
#include "CompanyDetailsWidget.h"
#include "GuiUtils.h"
#include "FileResourceAddWidget.h"
#include "NewDataDialog.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);

    connect(ui->clientsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddClient);
    connect(ui->templatesWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddTemplate);
    connect(ui->stylesheetsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddStylesheet);
    connect(ui->backButton, &QAbstractButton::clicked, this, &MorePage::back);

    connect(ui->titleBarWidget, &TitleBarWidget::settingsClicked, this, &MorePage::settingsClicked);
    connect(ui->titleBarWidget, &TitleBarWidget::aboutClicked, this, &MorePage::aboutClicked);
}

MorePage::~MorePage()
{
    delete ui;
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
