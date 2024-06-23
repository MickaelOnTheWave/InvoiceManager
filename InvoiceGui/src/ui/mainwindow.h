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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ClientModel.h"
#include "InvoiceModel.h"
#include "FileResourceModel.h"

#include "InvoiceDbController.h"
#include "DbStatusForm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <QStringListModel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCreateDb();
    void onOpenDb();
    void onOpenLastDb();
    void onCloseDb();
    void onSettings();
    void onAbout();

    void onFinishDbCreation();

    void onGoToCreateNewInvoice();
    void onGoToCreateNewInvoiceFromLast();
    void onGoToCharts();
    void onGoToMore();
    void onCreateNewInvoice();
    void onBackToMainPage();
    void onBackToPreviousPage();


private:
    void initializeOpenLastDb();
    void createModels();
    void switchToMainWidget();

    void showError(const QString& title, const QString& details);

    bool isDbOpeningConfirmed();

    void openDb(const QString& file);

    bool areChildCompaniesSeparated() const;

    Ui::MainWindow *ui;

    InvoiceDbController controller;
    ClientModel* clientModel;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
    InvoiceModel* invoiceModel;
    QWidget* previousPage;
};
#endif // MAINWINDOW_H
