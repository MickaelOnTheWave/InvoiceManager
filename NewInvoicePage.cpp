#include "NewInvoicePage.h"
#include "ui_NewInvoicePage.h"

#include <QDialogButtonBox>

NewInvoicePage::NewInvoicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewInvoicePage)
{
    ui->setupUi(this);

    connect(ui->closeBox, &QDialogButtonBox::accepted, this, &NewInvoicePage::create);
    connect(ui->closeBox, &QDialogButtonBox::rejected, this, &NewInvoicePage::cancel);
    connect(ui->clientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NewInvoicePage::onClientComboChange);
}

NewInvoicePage::~NewInvoicePage()
{
    delete ui;
}

void NewInvoicePage::connectModels(ClientModel *_clientModel, StylesheetModel *_stylesheetModel)
{
    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;
}

// TODO : This should be done with a focus enter event.
void NewInvoicePage::refreshData(const QString& companyName)
{
    ui->companyNameLabel->setText(companyName);
    resetComboData(ui->clientCombo, buildClientNames());
    resetComboData(ui->stylesheetCombo, stylesheetModel->getPathList());
}

void NewInvoicePage::onClientComboChange(int index)
{
    const CompanyData data = clientModel->getDataAtRow(index);
    ui->clientDetailsWidget->fill(data);
}

QStringList NewInvoicePage::buildClientNames() const
{
    QStringList names;
    for (int i=0; i<clientModel->rowCount(); ++i)
    {
        const QModelIndex index = clientModel->index(i, 1);
        names.append(clientModel->data(index).toString());
    }
    return names;
}

void NewInvoicePage::resetComboData(QComboBox *combo, const QStringList &newData)
{
    combo->clear();
    combo->addItems(newData);
}
