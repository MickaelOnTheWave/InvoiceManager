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

#ifndef NEWINVOICEPAGE_H
#define NEWINVOICEPAGE_H

#include <QWidget>

#include <QComboBox>

#include "ClientModel.h"
#include "InvoiceDbController.h"
#include <QtWebKit>
#include "FileResourceModel.h"

namespace Ui {
class NewInvoicePage;
}

class NewInvoicePage : public QWidget
{
    Q_OBJECT

public:
    explicit NewInvoicePage(QWidget *parent = nullptr);
    ~NewInvoicePage();

    void setController(InvoiceDbController* _controller);

    void connectViewsToModels(ClientModel* _clientModel,
                    FileResourceModel* _templateModel,
                    FileResourceModel* _stylesheetModel);
    void reset();
    void resetFromLast();

signals:
    void create();
    void cancel();
    void settingsClicked();
    void aboutClicked();

private slots:
    void onClientComboChange(int index);
    void onAddInvoiceDetail();
    void onCreateInvoice();
    void onCancel();

    void onTodayClicked();
    void onLastDayOfMonthClicked();
    void onCustomDateClicked();
    void onCustomDateUpdated(const QDate& newDate);
    void onGeneratePreviewClicked();

private:
    void insertTotalRow();
    void computeTotalRow();
    void resetInputData(const QString &companyName);
    void resetInvoiceData();

    void resetComboData(QComboBox* combobox, QAbstractItemModel* model);
    void updateDateEdit(const QDate& date);

    std::vector<InvoiceDetail> createDetailsCollection() const;

    /**
     * @brief writeInvoiceElements
     * @return list of element ids that have been written
     */
    std::vector<int> writeInvoiceElements();

    void addInvoiceDetail(const QString& name, const double value);

    int getComboIndex(QComboBox* combobox, const int id) const;

    QString getTemplateFile() const;
    QString getCssFile() const;

    InvoiceTemplateData createInvoiceTemplateData() const;

    void setError(const QString& description);

    Ui::NewInvoicePage *ui;
    ClientModel* clientModel;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
    QAbstractItemModel* invoiceDetailsModel;
    InvoiceDbController* controller;
};

#endif // NEWINVOICEPAGE_H
