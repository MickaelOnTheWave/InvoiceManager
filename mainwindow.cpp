#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlTableModel>

#include "NewInvoicePage.h"

// TODO Next :
// - Update SQL table model
//  - Add company data details
//  - Add table for invoice details
//  - Add table for invoice details -> invoice mapping
// - Add webview in new invoice
// - Implement remove button in data handler widget

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->createDbButton, &QPushButton::clicked, this, &MainWindow::onCreateDb);
    connect(ui->openDbButton, &QPushButton::clicked, this, &MainWindow::onOpenDb);
    connect(ui->quitButton, &QPushButton::clicked, this, &QMainWindow::close);

    connect(ui->createPage, &CreatePage::confirm, this, &MainWindow::onFinishDbCreation);
    connect(ui->createPage, &CreatePage::cancel, this, &MainWindow::onCloseDb);

    connect(ui->mainPage, &MainPage::createNewInvoice, this, &MainWindow::onGoToCreateNewInvoice);
    connect(ui->mainPage, &MainPage::goToMore, this, &MainWindow::onGoToMore);
    connect(ui->mainPage, &MainPage::closeDb, this, &MainWindow::onCloseDb);

    connect(ui->newInvoicePage, &NewInvoicePage::create, this, &MainWindow::onCreateNewInvoice);
    connect(ui->newInvoicePage, &NewInvoicePage::cancel, this, &MainWindow::onBackToMainPage);

    connect(ui->morePage, &MorePage::back, this, &MainWindow::onBackToMainPage);

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

        ui->stackedWidget->setCurrentWidget(ui->createPage);
        ui->createPage->setModel(stylesheetModel);
}

void MainWindow::onOpenDb()
{
    const QString title = tr("Select an existing Invoice Database file");
    const QString typeDescription = tr("Invoice Database (*.idb)");
    const QString dbFile = QFileDialog::getOpenFileName(this, title, QString(), typeDescription);
    if (!dbFile.isEmpty())
    {
        const bool ok = controller.openDb(dbFile);
        if (ok)
        {
            const int dbVersion = controller.getDatabaseVersion();
            if (dbVersion != controller.currentDbVersion)
            {
                const QString message("DB version mismatch. The app might crash while trying to handle it.\n"
                                      "DB version : %1 - Current version : %2");
                showError("Warning", message.arg(dbVersion).arg(controller.currentDbVersion));
            }
            createModels();
            switchToMainWidget();
        }
        else
            showError("Error", "Error while trying to open database file");
    }
}

void MainWindow::onCloseDb()
{
    controller.closeDb();
    ui->stackedWidget->setCurrentWidget(ui->startPage);
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

    if (!controller.writeStylesheets(stylesheetModel->getPathList()))
    {
        showError("Write stylesheets Table", controller.getLastError());
        ui->stackedWidget->setCurrentWidget(ui->startPage);
        return;
    }

    switchToMainWidget();
}

void MainWindow::onGoToCreateNewInvoice()
{
    ui->newInvoicePage->reset(controller.getCompanyName());
    ui->stackedWidget->setCurrentWidget(ui->newInvoicePage);
}

void MainWindow::onCreateNewInvoice()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::onBackToMainPage()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::onGoToMore()
{
    ui->stackedWidget->setCurrentWidget(ui->morePage);
}

void MainWindow::createModels()
{
    stylesheetModel = new StylesheetModel(this, controller.getDatabase());
    clientModel = new ClientModel(this);

    ui->mainPage->connectViewsToModels(clientModel, stylesheetModel);
    ui->morePage->connectViewsToModels(clientModel, stylesheetModel);
    ui->newInvoicePage->connectModels(clientModel, stylesheetModel);
}

void MainWindow::switchToMainWidget()
{
    ui->mainPage->setDisplayData(controller.getCompanyName(), controller.getDatabaseFile(),
                                 controller.getDatabaseVersion());
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::showError(const QString &title, const QString &details)
{
    QMessageBox::warning(this, title, details);
}

