#include "CreatePage.h"
#include "ui_CreatePage.h"

#include <QFileDialog>
#include "GuiUtils.h"

CreatePage::CreatePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreatePage)
{
    ui->setupUi(this);

    connect(ui->templatesDataWidget, &DataHandlerWidget::addClicked, this, &CreatePage::onAddTemplate);
    connect(ui->stylesheetsDataWidget, &DataHandlerWidget::addClicked, this, &CreatePage::onAddStylesheet);

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

void CreatePage::setModel(FileResourceModel *_templateModel, FileResourceModel *_stylesheetModel)
{
    templateModel = _templateModel;
    ui->templatesDataWidget->setModel(_templateModel);
    ui->templatesDataWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch});

    stylesheetModel = _stylesheetModel;
    ui->stylesheetsDataWidget->setModel(_stylesheetModel);
    ui->stylesheetsDataWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch});
}

void CreatePage::onAddStylesheet()
{
    GuiUtils::OnAddStylesheet(stylesheetModel);
}

void CreatePage::onAddTemplate()
{
    GuiUtils::OnAddTemplate(templateModel);
}
