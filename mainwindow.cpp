#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QPushButton>

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

    connect(ui->createWidget, &InitialDataForm::confirm, this, &MainWindow::onFinishDbCreation);
    connect(ui->createWidget, &InitialDataForm::cancel, this, &MainWindow::onCloseAndDiscardDb);

    connect(ui->mainWidget, &DatabaseForm::closeAndSave, this, &MainWindow::onCloseAndSaveDb);
    connect(ui->mainWidget, &DatabaseForm::closeAndDiscard, this, &MainWindow::onCloseAndDiscardDb);

    ui->stackedWidget->setCurrentWidget(ui->startWidget);
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
    if (!dbFile.isEmpty())
    {
        controller.createDb(dbFile);
        createModels();

        ui->stackedWidget->setCurrentWidget(ui->createWidget);
        ui->createWidget->setModel(stylesheetModel);
    }
}

void MainWindow::onOpenDb()
{
    const QString title = tr("Select an existing Invoice Database file");
    const QString typeDescription = tr("Invoice Database (*.idb)");
    const QString dbFile = QFileDialog::getOpenFileName(this, title, QString(), typeDescription);
    if (!dbFile.isEmpty())
    {
        controller.openDb(dbFile);

        ui->stackedWidget->setCurrentWidget(ui->mainWidget);
        ui->mainWidget->setCompanyName(controller.getCompanyName());
        //ui->mainWidget->setModel(stylesheetModel);
    }
}

void MainWindow::onCloseAndSaveDb()
{
    //controller.save();
    controller.closeDb();
    ui->stackedWidget->setCurrentWidget(ui->startWidget);
}

void MainWindow::onCloseAndDiscardDb()
{
    controller.closeDb();
    ui->stackedWidget->setCurrentWidget(ui->startWidget);
}

void MainWindow::onFinishDbCreation()
{
    controller.write(ui->createWidget->getCompanyName(), stylesheetModel);
    ui->stackedWidget->setCurrentWidget(ui->mainWidget);
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
}

