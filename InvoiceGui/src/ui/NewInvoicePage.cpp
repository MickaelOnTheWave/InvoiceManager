/*
 * InvoiceManager
 * Copyright (C) 2023 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    connect(ui->closeBox, &QDialogButtonBox::rejected, this, &NewInvoicePage::onCancel);
    connect(ui->clientCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NewInvoicePage::onClientComboChange);

    auto typedModel = new QStandardItemModel(this);
    typedModel->setColumnCount(4);
    invoiceDetailsModel = typedModel;
    invoiceDetailsModel->setHeaderData(0, Qt::Horizontal, tr("Service"));
    invoiceDetailsModel->setHeaderData(1, Qt::Horizontal, tr("Unitary Value"));
    invoiceDetailsModel->setHeaderData(2, Qt::Horizontal, tr("Quantity"));
    invoiceDetailsModel->setHeaderData(3, Qt::Horizontal, tr("Value"));

    resetInvoiceData();

    ui->invoiceDetailsWidget->setModel(invoiceDetailsModel);
    ui->invoiceDetailsWidget->setColumnsResizingMode({QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed});
    ui->invoiceDetailsWidget->setColumnsSizes({-1, 50, 70});

    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::addClicked, this, &NewInvoicePage::onAddInvoiceDetail);
    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::removeClicked, this, &NewInvoicePage::onRemoveInvoiceDetail);
    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::editingFinished, this, &NewInvoicePage::computeTotals);
    connect(ui->todayButton, &QAbstractButton::clicked, this, &NewInvoicePage::onTodayClicked);
    connect(ui->lastDayOfMonthButton, &QAbstractButton::clicked, this, &NewInvoicePage::onLastDayOfMonthClicked);
    connect(ui->customButton, &QAbstractButton::clicked, this, &NewInvoicePage::onCustomDateClicked);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &NewInvoicePage::onCustomDateUpdated);
    connect(ui->generateButton, &QAbstractButton::clicked, this, &NewInvoicePage::onGeneratePreviewClicked);

    connect(ui->titleBarWidget, &TitleBarWidget::settingsClicked, this, &NewInvoicePage::settingsClicked);
    connect(ui->titleBarWidget, &TitleBarWidget::aboutClicked, this, &NewInvoicePage::aboutClicked);

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

    const int nextId = controller->getLastInvoiceId() + 1;
    ui->invoiceIdBox->setValue(nextId);

    onGeneratePreviewClicked();
}

void NewInvoicePage::resetFromLast()
{
    resetInputData(controller->getUserCompanyName());
    invoiceDetailsModel->removeRows(0, invoiceDetailsModel->rowCount());
    insertTotalRow();

    const int nextId = controller->getLastInvoiceId() + 1;
    ui->invoiceIdBox->setValue(nextId);

    const InvoiceDbData data = controller->getLastInvoiceData();
    ui->clientCombo->setCurrentIndex(getComboIndex(ui->clientCombo, data.clientId));
    ui->templateCombo->setCurrentIndex(getComboIndex(ui->templateCombo, data.templateId));
    ui->stylesheetCombo->setCurrentIndex(getComboIndex(ui->stylesheetCombo, data.stylesheetId));

    ui->notesEdit->setText(data.notes);
    ui->currencyEdit->setText(data.currency);

    for (const int detailId : data.detailsIds)
    {
       const InvoiceDetail detail = controller->getInvoiceDetail(detailId);
        addInvoiceDetail(detail.description, detail.quantity, detail.value);
    }
    computeTotals();

    onGeneratePreviewClicked();
}

void NewInvoicePage::onClientComboChange(int index)
{
    const CompanyData data = clientModel->getDataAtRow(index);
    ui->clientDetailsWidget->fill(data);
}

void NewInvoicePage::onAddInvoiceDetail()
{
    addInvoiceDetail("Service", 0.0, 1.0);
    computeTotals();
}

void NewInvoicePage::onRemoveInvoiceDetail(const QModelIndex index)
{
   invoiceDetailsModel->removeRow(index.row());
   computeTotals();
}

void NewInvoicePage::onCreateInvoice()
{
   ui->errorLabel->hide();
    const int invoiceId = ui->invoiceIdBox->value();
    const int clientId = clientModel->getId(ui->clientCombo->currentIndex());
    const int templateId = templateModel->getId(ui->templateCombo->currentIndex());
    const int stylesheetId = stylesheetModel->getId(ui->stylesheetCombo->currentIndex());

    if (controller->invoiceExists(invoiceId))
    {
       setError(QString("Invoice with id %1 already exists").arg(invoiceId));
       return;
    }

    // TODO : Be smart. If line is exactly the same as previous invoice, we can reuse it.
    const std::vector<int> invoiceElementsIds = writeInvoiceElements();

    const bool ok = controller->writeInvoice(invoiceId, clientId, templateId, stylesheetId,
                                             invoiceElementsIds, ui->dateEdit->date(),
                                             ui->notesEdit->text(), ui->currencyEdit->text());
    if (!ok)
    {
       setError("Unexpected error while creating Invoice");
       // TODO : remove inserted invoice elements
       return;
    }

    emit create();
}

void NewInvoicePage::onCancel()
{
   ui->errorLabel->hide();
   emit cancel();
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
    invoiceDetailsModel->setData(invoiceDetailsModel->index(0, 1), 0);
    invoiceDetailsModel->setData(invoiceDetailsModel->index(0, 2), 0.00);
    invoiceDetailsModel->setData(invoiceDetailsModel->index(0, 3), 0.00);
    ui->invoiceDetailsWidget->setupTotalSpan();
}

void NewInvoicePage::computeTotals()
{
    double total = 0.0;
    for (int i=0; i<invoiceDetailsModel->rowCount()-1; ++i)
    {
      const double quantity = GetInvoiceDetailData(i, 1).toDouble();
      const double unitaryValue = GetInvoiceDetailData(i, 2).toDouble();
      const int rowTotal = quantity * unitaryValue;
      SetInvoiceDetailData(i, 3, rowTotal);
      invoiceDetailsModel->setData(invoiceDetailsModel->index(invoiceDetailsModel->rowCount()-1, 3), total);
      total += invoiceDetailsModel->data(invoiceDetailsModel->index(i, 3)).toDouble();
    }
    SetInvoiceDetailData(invoiceDetailsModel->rowCount()-1, 3, total);
}

void NewInvoicePage::resetInputData(const QString &companyName)
{
   ui->titleBarWidget->setTitle(companyName);
   resetComboData(ui->clientCombo, clientModel);
   resetComboData(ui->templateCombo, templateModel);
   resetComboData(ui->stylesheetCombo, stylesheetModel);
}

void NewInvoicePage::resetInvoiceData()
{
    ui->errorLabel->hide();
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
        const QString description = GetInvoiceDetailData(i, 0).toString();
        const double unitaryValue = GetInvoiceDetailData(i, 1).toDouble();
        const double quantity = GetInvoiceDetailData(i, 2).toDouble();
        details.emplace_back(description, quantity, unitaryValue);
    }
    return details;
}

std::vector<int> NewInvoicePage::writeInvoiceElements()
{
    const std::vector<InvoiceDetail> invoiceDetails = createDetailsCollection();
    return controller->writeInvoiceDetails(invoiceDetails);
}

void NewInvoicePage::addInvoiceDetail(const QString &name, const double value, const double quantity)
{
    const int newRowIndex = invoiceDetailsModel->rowCount()-1;
    invoiceDetailsModel->insertRow(newRowIndex);
    SetInvoiceDetailData(newRowIndex, 0, name);
    SetInvoiceDetailData(newRowIndex, 1, quantity);
    SetInvoiceDetailData(newRowIndex, 2, value);
    SetInvoiceDetailData(newRowIndex, 3, quantity * value);
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

QString NewInvoicePage::getTemplateContent() const
{
    const int id = ui->templateCombo->currentIndex() + 1;
    return controller->getTemplateData(id);
}

QString NewInvoicePage::getCssContent() const
{
    const int stylesheetId = ui->stylesheetCombo->currentIndex() + 1;
    return controller->getStylesheetData(stylesheetId);
}

InvoiceUserData NewInvoicePage::createInvoiceTemplateData() const
{
   InvoiceUserData data;
   data.templateData = getTemplateContent();
   data.stylesheetData = getCssContent();
   data.id = ui->invoiceIdBox->value();
   data.date = ui->dateEdit->date();
   data.userCompany = controller->getUserCompanyData();
   data.clientCompany = ui->clientDetailsWidget->getData();
   data.details = createDetailsCollection();
   data.currency = ui->currencyEdit->text();
   data.notes = ui->notesEdit->text();
   return data;
}

void NewInvoicePage::setError(const QString& description)
{
   ui->errorLabel->setText(description);
   ui->errorLabel->show();
}

QVariant NewInvoicePage::GetInvoiceDetailData(const int row, const int column) const
{
   return invoiceDetailsModel->data(invoiceDetailsModel->index(row, column));
}

void NewInvoicePage::SetInvoiceDetailData(const int row, const int column, const QVariant& data)
{
   invoiceDetailsModel->setData(invoiceDetailsModel->index(row, column), data);
}
