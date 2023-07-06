#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlTableModel>

#include "NewInvoicePage.h"

// TODO Next :
// - Add data models (even with dummy data) to have UI showing something
// - Add forms for New Invoice and More

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->createDbButton, &QPushButton::clicked, this, &MainWindow::onCreateDb);
    connect(ui->openDbButton, &QPushButton::clicked, this, &MainWindow::onOpenDb);
    connect(ui->quitButton, &QPushButton::clicked, this, &QMainWindow::close);

    connect(ui->createPage, &InitialDataForm::confirm, this, &MainWindow::onFinishDbCreation);
    connect(ui->createPage, &InitialDataForm::cancel, this, &MainWindow::onCloseAndDiscardDb);

    connect(ui->mainWidget, &DatabaseForm::createNewInvoice, this, &MainWindow::onGoToCreateNewInvoice);
    connect(ui->mainWidget, &DatabaseForm::goToMore, this, &MainWindow::onGoToMore);
    connect(ui->mainWidget, &DatabaseForm::closeAndSave, this, &MainWindow::onCloseAndSaveDb);
    connect(ui->mainWidget, &DatabaseForm::closeAndDiscard, this, &MainWindow::onCloseAndDiscardDb);

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

    if (!controller.writeStylesheets(stylesheetModel->stringList()))
    {
        showError("Write stylesheets Table", controller.getLastError());
        ui->stackedWidget->setCurrentWidget(ui->startPage);
        return;
    }

    switchToMainWidget();
}

void MainWindow::onGoToCreateNewInvoice()
{
    ui->stackedWidget->setCurrentWidget(ui->newInvoicePage);
}

void MainWindow::onCreateNewInvoice()
{
    ui->stackedWidget->setCurrentWidget(ui->mainWidget);
}

void MainWindow::onBackToMainPage()
{
    ui->stackedWidget->setCurrentWidget(ui->mainWidget);
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
    stylesheetModel = new QStringListModel(this);
    stylesheetModel->setHeaderData(0, Qt::Horizontal, "Stylesheet path");

    auto sqlClientModel = new QSqlTableModel(this, controller.getDatabase());
    sqlClientModel->setTable("client");
    sqlClientModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sqlClientModel->select();
    //sqlClientModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    //sqlClientModel->setHeaderData(1, Qt::Horizontal, tr("Address"));
    clientModel = sqlClientModel;

    ui->morePage->connectViewsToModels(clientModel, stylesheetModel);
}

void MainWindow::switchToMainWidget()
{
    ui->mainWidget->setCompanyName(controller.getCompanyName());
    //ui->mainWidget->setModel(stylesheetModel);

    ui->stackedWidget->setCurrentWidget(ui->mainWidget);
}

void MainWindow::showError(const QString &title, const QString &details)
{
    QMessageBox::warning(this, title, details);
}

