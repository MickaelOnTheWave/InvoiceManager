#include "InitialDataForm.h"
#include "ui_InitialDataForm.h"

#include <QFileDialog>

InitialDataForm::InitialDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InitialDataForm)
{
    ui->setupUi(this);

    connect(ui->stylesheedsDataWidget, &DataHandlerWidget::addClicked, this, &InitialDataForm::onAddStylesheet);

    connect(ui->confirmButton, &QAbstractButton::clicked, this, &InitialDataForm::confirm);
    connect(ui->cancelButton, &QAbstractButton::clicked, this, &InitialDataForm::cancel);
}

InitialDataForm::~InitialDataForm()
{
    delete ui;
}

void InitialDataForm::setModel(QAbstractItemModel *model)
{
    stylesheetModel = model;
    ui->stylesheedsDataWidget->setModel(stylesheetModel);
}

QString InitialDataForm::getCompanyName() const
{
    return ui->nameEdit->text();
}

QString InitialDataForm::getCompanyAddress() const
{
    return ui->addressLineEdit1->text() + "\n" +
           ui->addressLineEdit2->text() + "\n" +
            ui->addressLineEdit3->text();
}

QString InitialDataForm::getCompanyEmail() const
{
    return ui->emailEdit->text();
}

void InitialDataForm::onAddStylesheet()
{
    const QString title = tr("Select an Invoice Stylesheet");
    const QString typeDescription = tr("Stylesheet (*.css)");
    QString filePath = QFileDialog::getOpenFileName(this, title, "", typeDescription);
    if (!filePath.isEmpty())
    {
        insertIntoModel(filePath);
    }
}

void InitialDataForm::insertIntoModel(const QString &stylesheetPath)
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
