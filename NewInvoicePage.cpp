#include "NewInvoicePage.h"
#include "ui_NewInvoicePage.h"

#include <QDialogButtonBox>

#include "DataHandlerWidget.h"

NewInvoicePage::NewInvoicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewInvoicePage)
{
    ui->setupUi(this);

    connect(ui->closeBox, &QDialogButtonBox::accepted, this, &NewInvoicePage::onCreateInvoice);
    connect(ui->closeBox, &QDialogButtonBox::rejected, this, &NewInvoicePage::cancel);
    connect(ui->clientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NewInvoicePage::onClientComboChange);

    auto typedModel = new QStandardItemModel(this);
    typedModel->setColumnCount(2);
    invoiceDetailsModel = typedModel;
    invoiceDetailsModel->setHeaderData(0, Qt::Horizontal, tr("Service"));
    invoiceDetailsModel->setHeaderData(1, Qt::Horizontal, tr("Value"));

    resetInvoiceData();

    ui->invoiceDetailsWidget->setModel(invoiceDetailsModel);
    ui->invoiceDetailsWidget->setColumnsResizingMode({QHeaderView::Stretch, QHeaderView::Fixed});

    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::addClicked, this, &NewInvoicePage::onAddInvoiceDetail);
    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::editingFinished, this, &NewInvoicePage::computeTotalRow);

    connect(ui->todayButton, &QAbstractButton::clicked, this, &NewInvoicePage::onTodayClicked);
    connect(ui->lastDayOfMonthButton, &QAbstractButton::clicked, this, &NewInvoicePage::onLastDayOfMonthClicked);
    connect(ui->customButton, &QAbstractButton::clicked, this, &NewInvoicePage::onCustomDateClicked);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &NewInvoicePage::onCustomDateUpdated);

    ui->lastDayOfMonthButton->setChecked(true);
    onLastDayOfMonthClicked();
}

NewInvoicePage::~NewInvoicePage()
{
    delete ui;
}

void NewInvoicePage::initialize(ClientModel *_clientModel, StylesheetModel *_stylesheetModel,
                                InvoiceDbController *_controller)
{
    clientModel = _clientModel;
    stylesheetModel = _stylesheetModel;
    controller = _controller;
}

void NewInvoicePage::reset()
{
    resetInputData(controller->getUserCompanyName());
    resetInvoiceData();

    const int nextId = controller->getLastUsedInvoiceId() + 1;
    ui->invoiceIdBox->setValue(nextId);
}

void NewInvoicePage::resetFromLast()
{
    resetInputData(controller->getUserCompanyName());
    invoiceDetailsModel->removeRows(0, invoiceDetailsModel->rowCount());
    insertTotalRow();

    const int nextId = controller->getLastUsedInvoiceId() + 1;
    ui->invoiceIdBox->setValue(nextId);

    const InvoiceData data = controller->getLastInvoiceData();
    ui->clientCombo->setCurrentIndex(getClientIndex(data.clientId));
    ui->stylesheetCombo->setCurrentIndex(getStylesheetIndex(data.stylesheetId));
    for (const auto detail : data.details)
        addInvoiceDetail(detail.description, detail.value);
    computeTotalRow();
}

void NewInvoicePage::onClientComboChange(int index)
{
    const CompanyData data = clientModel->getDataAtRow(index);
    ui->clientDetailsWidget->fill(data);
}

void NewInvoicePage::onAddInvoiceDetail()
{
    addInvoiceDetail("Service", 0.00);
    computeTotalRow();
}

void NewInvoicePage::onCreateInvoice()
{
    const int invoiceId = ui->invoiceIdBox->value();
    const int clientId = clientModel->getId(ui->clientCombo->currentIndex());
    const int stylesheetId = stylesheetModel->getId(ui->stylesheetCombo->currentIndex());
    const std::vector<int> invoiceElementsIds = writeInvoiceElements();

    const bool ok = controller->writeInvoice(invoiceId, clientId, stylesheetId, invoiceElementsIds, ui->dateEdit->date());
    if (ok)
        emit create();
}

void NewInvoicePage::onTodayClicked()
{
    ui->dateEdit->setEnabled(false);
    updateDateEdit(QDate::currentDate());
}

void NewInvoicePage::onLastDayOfMonthClicked()
{
    ui->dateEdit->setEnabled(false);

    const auto today = QDate::currentDate();
    const QDate selectedDate(today.year(), today.month(), today.daysInMonth());
    updateDateEdit(selectedDate);
}

void NewInvoicePage::onCustomDateClicked()
{
    ui->dateEdit->setEnabled(true);
    updateDateEdit(ui->dateEdit->date());
}

void NewInvoicePage::onCustomDateUpdated(const QDate &newDate)
{
    ui->dateLabel->setText(newDate.toString());
}

void NewInvoicePage::insertTotalRow()
{
    invoiceDetailsModel->insertRow(0);
    invoiceDetailsModel->setData(invoiceDetailsModel->index(0, 0), "Total");
    invoiceDetailsModel->setData(invoiceDetailsModel->index(0, 1), 0.00);
}

void NewInvoicePage::computeTotalRow()
{
    double total = 0.0;
    for (int i=0; i<invoiceDetailsModel->rowCount()-1; ++i)
    {
        total += invoiceDetailsModel->data(invoiceDetailsModel->index(i, 1)).toDouble();
    }
    invoiceDetailsModel->setData(invoiceDetailsModel->index(invoiceDetailsModel->rowCount()-1, 1), total);
}

void NewInvoicePage::resetInputData(const QString &companyName)
{
    ui->companyNameLabel->setText(companyName);
    resetComboData(ui->clientCombo, buildClientNames());
    resetComboData(ui->stylesheetCombo, stylesheetModel->getNameList());
}

void NewInvoicePage::resetInvoiceData()
{
    invoiceDetailsModel->removeRows(0, invoiceDetailsModel->rowCount());
    insertTotalRow();
    onAddInvoiceDetail();
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

void NewInvoicePage::updateDateEdit(const QDate &date)
{
    ui->dateEdit->setDate(date);
    ui->dateLabel->setText(date.toString());
}

std::vector<InvoiceDetail> NewInvoicePage::createDetailsCollection() const
{
    std::vector<InvoiceDetail> details;
    const int detailCount = invoiceDetailsModel->rowCount() - 1;
    for (int i=0; i<detailCount; ++i)
    {
        const QString description = invoiceDetailsModel->data(invoiceDetailsModel->index(i, 0)).toString();
        const double value = invoiceDetailsModel->data(invoiceDetailsModel->index(i, 1)).toDouble();
        details.emplace_back(description, value);
    }
    return details;
}

std::vector<int> NewInvoicePage::writeInvoiceElements()
{
    const std::vector<InvoiceDetail> invoiceDetails = createDetailsCollection();
    return controller->writeInvoiceDetails(invoiceDetails);
}

void NewInvoicePage::addInvoiceDetail(const QString &name, const double value)
{
    const int newRowIndex = invoiceDetailsModel->rowCount()-1;
    invoiceDetailsModel->insertRow(newRowIndex);
    invoiceDetailsModel->setData(invoiceDetailsModel->index(newRowIndex, 0), name);
    invoiceDetailsModel->setData(invoiceDetailsModel->index(newRowIndex, 1), value);
}

int NewInvoicePage::getClientIndex(const int id)
{
    // TODO : do a real computation
    return id - 2;
}

int NewInvoicePage::getStylesheetIndex(const int id)
{
    // TODO : do a real computation, but this is less important (data is easily consistent)
    return id - 1;
}
