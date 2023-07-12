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

void NewInvoicePage::connectModels(ClientModel *_clientModel)
{
    clientModel = _clientModel;
}

// TODO : This should be done with a focus enter event.
void NewInvoicePage::refreshData()
{
    const QStringList clientNames = buildNames();
    ui->clientCombo->clear();
    ui->clientCombo->addItems(clientNames);
}

void NewInvoicePage::onClientComboChange(int index)
{
    const CompanyData data = clientModel->getDataAtRow(index);
    ui->clientDetailsWidget->fill(data);
}

QStringList NewInvoicePage::buildNames() const
{
    QStringList names;
    for (int i=0; i<clientModel->rowCount(); ++i)
    {
        const QModelIndex index = clientModel->index(i, 1);
        names.append(clientModel->data(index).toString());
    }
    return names;
}
