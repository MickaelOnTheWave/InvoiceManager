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
    ui->invoiceDetailsWidget->setColumnsSizes({-1, 80});

    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::addClicked, this, &NewInvoicePage::onAddInvoiceDetail);
    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::editingFinished, this, &NewInvoicePage::computeTotalRow);

    connect(ui->todayButton, &QAbstractButton::clicked, this, &NewInvoicePage::onTodayClicked);
    connect(ui->lastDayOfMonthButton, &QAbstractButton::clicked, this, &NewInvoicePage::onLastDayOfMonthClicked);
    connect(ui->customButton, &QAbstractButton::clicked, this, &NewInvoicePage::onCustomDateClicked);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &NewInvoicePage::onCustomDateUpdated);
    connect(ui->generateButton, &QAbstractButton::clicked, this, &NewInvoicePage::onGeneratePreviewClicked);

    ui->lastDayOfMonthButton->setChecked(true);
    onLastDayOfMonthClicked();
}

NewInvoicePage::~NewInvoicePage()
{
   delete ui;
}

void NewInvoicePage::setController(InvoiceDbController* _controller)
{
   controller = _controller;
}

void NewInvoicePage::connectViewsToModels(ClientModel *_clientModel, FileResourceModel *_templateModel,
                                FileResourceModel *_stylesheetModel)
{
    clientModel = _clientModel;
    templateModel = _templateModel;
    stylesheetModel = _stylesheetModel;
}

void NewInvoicePage::reset()
{
    resetInputData(controller->getUserCompanyName());
    resetInvoiceData();

    ui->notesEdit->setText("");
    ui->currencyEdit->setText("€");

    const int nextId = controller->getLastUsedInvoiceId() + 1;
    ui->invoiceIdBox->setValue(nextId);

    onGeneratePreviewClicked();
}

void NewInvoicePage::resetFromLast()
{
    resetInputData(controller->getUserCompanyName());
    invoiceDetailsModel->removeRows(0, invoiceDetailsModel->rowCount());
    insertTotalRow();

    const int nextId = controller->getLastUsedInvoiceId() + 1;
    ui->invoiceIdBox->setValue(nextId);

    const InvoiceData data = controller->getLastInvoiceData();
    ui->clientCombo->setCurrentIndex(getComboIndex(ui->clientCombo, data.clientId));
    ui->templateCombo->setCurrentIndex(getComboIndex(ui->templateCombo, data.templateId));
    ui->stylesheetCombo->setCurrentIndex(getComboIndex(ui->stylesheetCombo, data.stylesheetId));

    ui->notesEdit->setText(data.notes);
    ui->currencyEdit->setText(data.currency);

    for (const auto detail : data.details)
        addInvoiceDetail(detail.description, detail.value);
    computeTotalRow();

    onGeneratePreviewClicked();
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
    const int templateId = templateModel->getId(ui->templateCombo->currentIndex());
    const int stylesheetId = stylesheetModel->getId(ui->stylesheetCombo->currentIndex());
    const std::vector<int> invoiceElementsIds = writeInvoiceElements();

    const bool ok = controller->writeInvoice(invoiceId, clientId, templateId, stylesheetId,
                                             invoiceElementsIds, ui->dateEdit->date(),
                                             ui->notesEdit->text(), ui->currencyEdit->text());
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

void NewInvoicePage::onGeneratePreviewClicked()
{
   ui->previewWidget->show(createInvoiceTemplateData());
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
    resetComboData(ui->clientCombo, clientModel);
    resetComboData(ui->templateCombo, templateModel);
    resetComboData(ui->stylesheetCombo, stylesheetModel);
}

void NewInvoicePage::resetInvoiceData()
{
    invoiceDetailsModel->removeRows(0, invoiceDetailsModel->rowCount());
    insertTotalRow();
    onAddInvoiceDetail();
}

void NewInvoicePage::resetComboData(QComboBox *combobox, QAbstractItemModel *model)
{
    combobox->clear();
    for (int i=0; i<model->rowCount(); ++i)
    {
        const QVariant id = model->data(model->index(i, 0));
        const QString name = model->data(model->index(i, 1)).toString();
        combobox->addItem(name, id);
    }
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

int NewInvoicePage::getComboIndex(QComboBox *combobox, const int id) const
{
    for (int index=0; index<combobox->count(); ++index)
    {
        const int currentId = combobox->itemData(index).toInt();
        if (currentId == id)
            return index;
    }
    return -1;
}

QString NewInvoicePage::getTemplateFile() const
{
    const int id = ui->templateCombo->currentIndex() + 1;
    return controller->getTemplateFilename(id);
}

QString NewInvoicePage::getCssFile() const
{
    const int stylesheetId = ui->stylesheetCombo->currentIndex() + 1;
    return controller->getStylesheetFilename(stylesheetId);
}

InvoiceTemplateData NewInvoicePage::createInvoiceTemplateData() const
{
   InvoiceTemplateData data;
   data.templatePath = getTemplateFile();
   data.stylesheetPath = getCssFile();
   data.id = ui->invoiceIdBox->value();
   data.date = ui->dateEdit->date();
   data.userCompany = controller->getUserCompanyData();
   data.clientCompany = ui->clientDetailsWidget->getData();
   data.details = createDetailsCollection();
   data.currency = ui->currencyEdit->text();
   data.notes = ui->notesEdit->text();
   return data;
}
