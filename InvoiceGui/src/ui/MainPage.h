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

#ifndef MainPage_H
#define MainPage_H

#include <QTableView>
#include <QWidget>

#include "InvoiceDbController.h"
#include "InvoiceData.h"

#include "ClientModel.h"
#include "InvoiceModel.h"

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void update();

    void setDisplayData(const QString &companyName, const QString& dbFile,
                        const int dbVersion);

    void setController(InvoiceDbController* _controller);

    void connectViewsToModels(ClientModel* _clientModel,
                              QAbstractItemModel* _templateModel,
                              QAbstractItemModel* _stylesheetModel,
                              InvoiceModel* _invoiceModel);

signals:
    void closeDb();
    void createNewInvoice();
    void createNewInvoiceFromLast();
    void goToMore();
    void settingsClicked();
    void aboutClicked();

private slots:
    void onOpenInvoice(const QModelIndex& index);
    void onOpenClient(const QModelIndex& index);
    void onRemoveInvoice(const int id);
    void onCompanyUpdateCreated();

    void onInvoiceHeaderClicked(int logicalIndex);

private:
    void initializeFileResourceView(QTableView* viewControl);
    void setViewDefaults(QTableView* view);

    InvoiceUserData createInvoiceTemplateData(const QModelIndex& index);
    CompanyData createCompanyData(const QModelIndex& index);

    Ui::MainPage *ui;
    ClientModel* clientModel;
    QAbstractItemModel* templateModel;
    QAbstractItemModel* stylesheetModel;
    InvoiceModel* invoiceModel;
    InvoiceDbController* controller = nullptr;
};

#endif // MainPage_H
