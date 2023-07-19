#include "CreatePage.h"
#include "ui_CreatePage.h"

#include <QFileDialog>
#include "GuiUtils.h"

CreatePage::CreatePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreatePage)
{
    ui->setupUi(this);

    connect(ui->stylesheedsDataWidget, &DataHandlerWidget::addClicked, this, &CreatePage::onAddStylesheet);

    connect(ui->confirmButton, &QAbstractButton::clicked, this, &CreatePage::confirm);
    connect(ui->cancelButton, &QAbstractButton::clicked, this, &CreatePage::cancel);
}

CreatePage::~CreatePage()
{
    delete ui;
}

void CreatePage::reset()
{
    ui->addressWidget->reset();
}

CompanyData CreatePage::getCompanyData() const
{
    return ui->addressWidget->getData();
}

void CreatePage::setModel(StylesheetModel *model)
{
    stylesheetModel = model;
    ui->stylesheedsDataWidget->setModel(stylesheetModel);
    ui->stylesheedsDataWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch});
}

void CreatePage::onAddStylesheet()
{
    GuiUtils::OnAddStylesheet(stylesheetModel);
}
