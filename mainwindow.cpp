#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlTableModel>

#include "NewInvoicePage.h"

// TODO Next :
// - Add forms for New Invoice and More

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->createDbButton, &QPushButton::clicked, this, &MainWindow::onCreateDb);
    connect(ui->openDbButton, &QPushButton::clicked, this, &MainWindow::onOpenDb);
    connect(ui->quitButton, &QPushButton::clicked, this, &QMainWindow::close);

    connect(ui->createPage, &CreatePage::confirm, this, &MainWindow::onFinishDbCreation);
    connect(ui->createPage, &CreatePage::cancel, this, &MainWindow::onCloseAndDiscardDb);

    connect(ui->mainPage, &MainPage::createNewInvoice, this, &MainWindow::onGoToCreateNewInvoice);
    connect(ui->mainPage, &MainPage::goToMore, this, &MainWindow::onGoToMore);
    connect(ui->mainPage, &MainPage::closeAndSave, this, &MainWindow::onCloseAndSaveDb);
    connect(ui->mainPage, &MainPage::closeAndDiscard, this, &MainWindow::onCloseAndDiscardDb);

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
        controller.openDb(dbFile);
        createModels();
        switchToMainWidget();
    }
}

void MainWindow::onCloseAndSaveDb()
{
    //controller.save();
    controller.closeDb();
    ui->stackedWidget->setCurrentWidget(ui->startPage);
}

void MainWindow::onCloseAndDiscardDb()
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
    ui->newInvoicePage->refreshData(controller.getCompanyName());
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

void MainWindow::connectDbStatusControls(DbStatusForm *dbStatusForm)
{
    connect(dbStatusForm, &DbStatusForm::closeAndSave, this, &MainWindow::onCloseAndSaveDb);
    connect(dbStatusForm, &DbStatusForm::closeAndDiscard, this, &MainWindow::onCloseAndDiscardDb);
}

void MainWindow::createModels()
{
    stylesheetModel = new StylesheetModel(this, controller.getDatabase());
    clientModel = new ClientModel(this, controller.getDatabase());

    ui->mainPage->connectViewsToModels(clientModel, stylesheetModel);
    ui->morePage->connectViewsToModels(clientModel, stylesheetModel);
    ui->newInvoicePage->connectModels(clientModel, stylesheetModel);
}

void MainWindow::switchToMainWidget()
{
    ui->mainPage->setCompanyName(controller.getCompanyName());
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::showError(const QString &title, const QString &details)
{
    QMessageBox::warning(this, title, details);
}

