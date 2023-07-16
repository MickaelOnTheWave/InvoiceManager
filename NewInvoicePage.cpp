#include "NewInvoicePage.h"
#include "ui_NewInvoicePage.h"

#include <QDialogButtonBox>

#include "DataHandlerWidget.h"

NewInvoicePage::NewInvoicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewInvoicePage)
{
    ui->setupUi(this);

    connect(ui->closeBox, &QDialogButtonBox::accepted, this, &NewInvoicePage::create);
    connect(ui->closeBox, &QDialogButtonBox::rejected, this, &NewInvoicePage::cancel);
    connect(ui->clientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NewInvoicePage::onClientComboChange);

    auto typedModel = new QStandardItemModel(this);
    typedModel->setColumnCount(2);
    invoiceDetailsModel = typedModel;
    invoiceDetailsModel->setHeaderData(0, Qt::Horizontal, tr("Service"));
    invoiceDetailsModel->setHeaderData(1, Qt::Horizontal, tr("Value"));

    onAddInvoiceDetail();
    ui->invoiceDetailsWidget->setModel(invoiceDetailsModel);
    ui->invoiceDetailsWidget->setColumnsResizingMode({QHeaderView::Stretch, QHeaderView::Fixed});

    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::addClicked, this, &NewInvoicePage::onAddInvoiceDetail);
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

void NewInvoicePage::reset(const QString& companyName)
{
    resetInputData(companyName);
    resetInvoiceData();
}

void NewInvoicePage::onClientComboChange(int index)
{
    const CompanyData data = clientModel->getDataAtRow(index);
    ui->clientDetailsWidget->fill(data);
}

void NewInvoicePage::onAddInvoiceDetail()
{
    const int newRowIndex = invoiceDetailsModel->rowCount();
    invoiceDetailsModel->insertRow(newRowIndex);
    invoiceDetailsModel->setData(invoiceDetailsModel->index(newRowIndex, 0), "Service");
    invoiceDetailsModel->setData(invoiceDetailsModel->index(newRowIndex, 1), 0.00);
}

void NewInvoicePage::resetInputData(const QString &companyName)
{
    ui->companyNameLabel->setText(companyName);
    resetComboData(ui->clientCombo, buildClientNames());
    resetComboData(ui->stylesheetCombo, stylesheetModel->getPathList());
}

void NewInvoicePage::resetInvoiceData()
{
    invoiceDetailsModel->removeRows(0, invoiceDetailsModel->rowCount());
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
