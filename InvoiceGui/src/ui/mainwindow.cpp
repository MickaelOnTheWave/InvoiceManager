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

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlTableModel>
#include <QSettings>

#include "DbMigrator.h"
#include "ChartsPage.h"
#include "NewInvoicePage.h"

// TODO Not that important :
// - Add versioning to clients / company

const QString lastDbKey = "lastdbfile";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->createDbButton, &QPushButton::clicked, this, &MainWindow::onCreateDb);
    connect(ui->openDbButton, &QPushButton::clicked, this, &MainWindow::onOpenDb);
    connect(ui->openLastDbButton, &QPushButton::clicked, this, &MainWindow::onOpenLastDb);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettings);
    connect(ui->aboutButton, &QPushButton::clicked, this, &MainWindow::onAbout);
    connect(ui->quitButton, &QPushButton::clicked, this, &QMainWindow::close);

    connect(ui->createPage, &CreatePage::confirm, this, &MainWindow::onFinishDbCreation);
    connect(ui->createPage, &CreatePage::cancel, this, &MainWindow::onCloseDb);

    connect(ui->mainPage, &MainPage::createNewInvoice, this, &MainWindow::onGoToCreateNewInvoice);
    connect(ui->mainPage, &MainPage::createNewInvoiceFromLast, this, &MainWindow::onGoToCreateNewInvoiceFromLast);
    connect(ui->mainPage, &MainPage::goToCharts, this, &MainWindow::onGoToCharts);
    connect(ui->mainPage, &MainPage::goToMore, this, &MainWindow::onGoToMore);
    connect(ui->mainPage, &MainPage::closeDb, this, &MainWindow::onCloseDb);
    connect(ui->mainPage, &MainPage::settingsClicked, this, &MainWindow::onSettings);
    connect(ui->mainPage, &MainPage::aboutClicked, this, &MainWindow::onAbout);

    connect(ui->newInvoicePage, &NewInvoicePage::create, this, &MainWindow::onCreateNewInvoice);
    connect(ui->newInvoicePage, &NewInvoicePage::cancel, this, &MainWindow::onBackToMainPage);
    connect(ui->newInvoicePage, &NewInvoicePage::settingsClicked, this, &MainWindow::onSettings);
    connect(ui->newInvoicePage, &NewInvoicePage::aboutClicked, this, &MainWindow::onAbout);

    connect(ui->chartsPage, &ChartsPage::back, this, &MainWindow::onBackToMainPage);

    connect(ui->morePage, &MorePage::back, this, &MainWindow::onBackToMainPage);
    connect(ui->morePage, &MorePage::settingsClicked, this, &MainWindow::onSettings);
    connect(ui->morePage, &MorePage::aboutClicked, this, &MainWindow::onAbout);

    connect(ui->settingsPage, &SettingsPage::done, this, &MainWindow::onBackToPreviousPage);
    connect(ui->aboutPage, &AboutPage::backClicked, this, &MainWindow::onBackToPreviousPage);

    initializeOpenLastDb();
    ui->aboutPage->setVersions(APP_VERSION, InvoiceDbController::currentDbVersion);

    ui->stackedWidget->setCurrentWidget(ui->startPage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCreateDb()
{
    const QString title = tr("Select an existing Invoice Database file");
    const QString typeDescription = tr("Invoice Database (*.idb)");
    const QString dbFile = QFileDialog::getSaveFileName(this, title, QString(), typeDescription);
    if (dbFile.isEmpty())
    {
        showError("Error", "Can't create an Invoice Database without a file");
        return;
    }

    if (!controller.createDb(dbFile))
    {
        showError("Database Creation", controller.getLastError());
        return;
    }

        createModels();
        ui->createPage->reset();
        ui->stackedWidget->setCurrentWidget(ui->createPage);
        ui->createPage->setModel(templateModel, stylesheetModel);
}

void MainWindow::onOpenDb()
{
    const QString title = tr("Select an existing Invoice Database file");
    const QString typeDescription = tr("Invoice Database (*.idb)");
    const QString dbFile = QFileDialog::getOpenFileName(this, title, QString(), typeDescription);
    openDb(dbFile);
}

void MainWindow::onOpenLastDb()
{
   QSettings settings;
   const QString dbFile = settings.value(lastDbKey).toString();
   openDb(dbFile);
}

void MainWindow::onCloseDb()
{
    controller.closeDb();

    initializeOpenLastDb();
    ui->stackedWidget->setCurrentWidget(ui->startPage);
}

void MainWindow::onSettings()
{
   previousPage = ui->stackedWidget->currentWidget();
   ui->settingsPage->refresh();
   ui->stackedWidget->setCurrentWidget(ui->settingsPage);
}

void MainWindow::onAbout()
{
   previousPage = ui->stackedWidget->currentWidget();
   ui->stackedWidget->setCurrentWidget(ui->aboutPage);
}

void MainWindow::onFinishDbCreation()
{
    CompanyData userCompany = ui->createPage->getCompanyData();
    if (!controller.writeUserCompany(userCompany))
    {
        showError("Write User Company Table", controller.getLastError());
        ui->stackedWidget->setCurrentWidget(ui->startPage);
        return;
    }

    switchToMainWidget();
}

void MainWindow::onGoToCreateNewInvoice()
{
    ui->newInvoicePage->reset();
    ui->stackedWidget->setCurrentWidget(ui->newInvoicePage);
}

void MainWindow::onGoToCreateNewInvoiceFromLast()
{
    ui->newInvoicePage->resetFromLast();
    ui->stackedWidget->setCurrentWidget(ui->newInvoicePage);
}

void MainWindow::onCreateNewInvoice()
{
    invoiceModel->refresh();
    onBackToMainPage();
}

void MainWindow::onBackToMainPage()
{
   ui->mainPage->update();
   ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::onBackToPreviousPage()
{
   ui->stackedWidget->setCurrentWidget(previousPage);

   clientModel->setSeparateChildCompanies(areChildCompaniesSeparated());
   clientModel->refreshModel();
}

void MainWindow::initializeOpenLastDb()
{
   QSettings settings;
   const bool hasLastDbKey = settings.contains(lastDbKey);
   if (hasLastDbKey)
   {
      QFileInfo lastDbFile(settings.value("lastdbfile").toString());

      const QString labelText = tr("Open Last Db (%1)").arg(lastDbFile.fileName());
      ui->openLastDbButton->setText(labelText);
   }
   ui->openLastDbButton->setEnabled(hasLastDbKey);
}

void MainWindow::onGoToCharts()
{
   ui->chartsPage->update();
   ui->stackedWidget->setCurrentWidget(ui->chartsPage);
}

void MainWindow::onGoToMore()
{
    ui->stackedWidget->setCurrentWidget(ui->morePage);
}

void MainWindow::createModels()
{
    templateModel = new FileResourceModel(InvoiceDbController::templateTypeId, this);
    stylesheetModel = new FileResourceModel(InvoiceDbController::stylesheetTypeId, this);
    clientModel = new ClientModel(&controller, areChildCompaniesSeparated(), this);
    invoiceModel = new InvoiceModel(this);

    ui->mainPage->setController(&controller);
    ui->newInvoicePage->setController(&controller);
    ui->chartsPage->setController(&controller);

    ui->mainPage->connectViewsToModels(clientModel, templateModel, stylesheetModel, invoiceModel);
    ui->morePage->connectViewsToModels(clientModel, templateModel, stylesheetModel);
    ui->newInvoicePage->connectViewsToModels(clientModel, templateModel, stylesheetModel);
}

void MainWindow::switchToMainWidget()
{
   QSettings settings;
   settings.setValue(lastDbKey, controller.getDatabaseFile());

   const QString userCompanyName = controller.getUserCompanyName();
   ui->mainPage->setDisplayData(userCompanyName, controller.getDatabaseFile(),
                              controller.getDatabaseVersion());
   ui->morePage->setCompanyTitle(userCompanyName);
   onBackToMainPage();
}

void MainWindow::showError(const QString &title, const QString &details)
{
    QMessageBox::warning(this, title, details);
}

bool MainWindow::isDbOpeningConfirmed()
{
   const int dbVersion = controller.getDatabaseVersion();
   if (dbVersion != controller.currentDbVersion)
   {
      QMessageBox msgBox;
     const QString message("DB version mismatch. The app might crash while trying to handle it.\n"
                           "DB version : %1 - Current version : %2\n"
                           "What to you want to do ?");
     auto* buttonMigrate = msgBox.addButton("Migrate", QMessageBox::YesRole);
     msgBox.addButton("Close", QMessageBox::NoRole);
     msgBox.setWindowTitle("Warning");
     msgBox.setText(message.arg(dbVersion).arg(controller.currentDbVersion));
     msgBox.exec();
     if (msgBox.clickedButton() == buttonMigrate)
     {
        DbMigrator migrator(controller);
        const bool ok = migrator.Migrate();
        if (!ok)
           showError("Database Migration", "Error trying to migrate Database");
        return ok;
     }
     else
        return false;
   }
   return true;
}

void MainWindow::openDb(const QString& file)
{
   if (file.isEmpty())
      return;

   const bool ok = controller.openDb(file);
   if (!ok)
   {
      showError("Error", "Error while trying to open database file");
      return;
   }

   if (!isDbOpeningConfirmed())
      return;

   createModels();
   switchToMainWidget();
}

bool MainWindow::areChildCompaniesSeparated() const
{
   QSettings settings;
   return settings.value("user/showchildcompanies", false).toBool();
}
