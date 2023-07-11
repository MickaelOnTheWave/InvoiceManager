#include "CreatePage.h"
#include "ui_CreatePage.h"

#include <QFileDialog>

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

CompanyData CreatePage::getCompanyData() const
{
    return ui->addressWidget->getData();
}

void CreatePage::setModel(QAbstractItemModel *model)
{
    stylesheetModel = model;
    ui->stylesheedsDataWidget->setModel(stylesheetModel);
}

void CreatePage::onAddStylesheet()
{
    const QString title = tr("Select an Invoice Stylesheet");
    const QString typeDescription = tr("Stylesheet (*.css)");
    QString filePath = QFileDialog::getOpenFileName(this, title, "", typeDescription);
    if (!filePath.isEmpty())
    {
        insertIntoModel(filePath);
    }
}

void CreatePage::insertIntoModel(const QString &stylesheetPath)
{
    QModelIndex parentIndex;
    int rowCount = stylesheetModel->rowCount(parentIndex);
    stylesheetModel->insertRow(rowCount, parentIndex);  // Insert a new row at the end of the model

    QModelIndex index = stylesheetModel->index(rowCount, 0, parentIndex);  // Get the model index for the inserted row and column 0
    stylesheetModel->setData(index, stylesheetPath);

    stylesheetModel->setData(parentIndex, "lala");

    // Insert the string into the model at the end
    //stylesheetModel->insertRow(rowCount, parentIndex);  // Insert a new row at the end of the model
    //QModelIndex index = stylesheetModel->index(rowCount, 0, parentIndex);  // Get the model index for the inserted row and column 0

    // Set the data for the index with the QString
    //stylesheetModel->setData(index, stylesheetPath);
}
