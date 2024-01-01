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

    invoiceDetailsModel = new InvoiceDetailsModel();

    resetInvoiceData();

    ui->invoiceDetailsWidget->setModel(invoiceDetailsModel);
    ui->invoiceDetailsWidget->setColumnsResizingMode({QHeaderView::Stretch, QHeaderView::Fixed, QHeaderView::Fixed});
    ui->invoiceDetailsWidget->setColumnsSizes({-1, 50, 70});

    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::addClicked, this, &NewInvoicePage::onAddInvoiceDetail);
    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::removeClicked, this, &NewInvoicePage::onRemoveInvoiceDetail);
    connect(ui->invoiceDetailsWidget, &DataHandlerWidget::editingFinished, this, &NewInvoicePage::refreshInvoice);
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
       invoiceDetailsModel->addDetail(detail.description, detail.quantity, detail.value);
    }

    refreshInvoice();
}

void NewInvoicePage::onClientComboChange(int index)
{
    const CompanyData data = clientModel->getDataAtRow(index);
    ui->clientDetailsWidget->fill(data);
}

void NewInvoicePage::onAddInvoiceDetail()
{
    invoiceDetailsModel->addDetail("Service", 0.0, 1.0);
    refreshInvoice();
}

void NewInvoicePage::onRemoveInvoiceDetail(const QModelIndex index)
{
   invoiceDetailsModel->removeRow(index.row());
   refreshInvoice();
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
    invoiceDetailsModel->addTotalRow();
    ui->invoiceDetailsWidget->setupTotalSpan();
}

void NewInvoicePage::refreshInvoice()
{
   invoiceDetailsModel->computeTotals();
   onGeneratePreviewClicked();
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
    invoiceDetailsModel->addDetail("Service", 0.0, 1.0);
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

std::vector<int> NewInvoicePage::writeInvoiceElements()
{
    const std::vector<InvoiceDetail> invoiceDetails = invoiceDetailsModel->toDetailsCollection();
    return controller->writeInvoiceDetails(invoiceDetails);
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
   data.details = invoiceDetailsModel->toDetailsCollection();
   data.currency = ui->currencyEdit->text();
   data.notes = ui->notesEdit->text();
   return data;
}

void NewInvoicePage::setError(const QString& description)
{
   ui->errorLabel->setText(description);
   ui->errorLabel->show();
}
