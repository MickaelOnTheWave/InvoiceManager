#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QPushButton>

#include "StylesheedFileForm.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->createDbButton, &QPushButton::clicked, this, &MainWindow::onCreateDb);
    connect(ui->openDbButton, &QPushButton::clicked, this, &MainWindow::onOpenDb);
    connect(ui->quitButton, &QPushButton::clicked, this, &QMainWindow::close);

    connectDbStatusControls(ui->dbStatusWidget);
    connectDbStatusControls(ui->dbStatusWidget_2);

    connect(ui->addStylesheetButton, &QAbstractButton::clicked, this, &MainWindow::onAddStylesheet);

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
        ui->stackedWidget->setCurrentWidget(ui->createWidget);
        ui->dbStatusWidget->setDbFile(dbFile);
    }
}

void MainWindow::onOpenDb()
{
    const QString title = tr("Select an existing Invoice Database file");
    const QString typeDescription = tr("Invoice Database (*.idb)");
    const QString dbFile = QFileDialog::getOpenFileName(this, title, QString(), typeDescription);
    if (!dbFile.isEmpty())
    {
        ui->stackedWidget->setCurrentWidget(ui->mainWidget);
        ui->dbStatusWidget_2->setDbFile(dbFile);
    }
}

void MainWindow::onCloseAndSaveDb()
{
    controller.save();
    controller.closeDb();
    ui->stackedWidget->setCurrentWidget(ui->startWidget);
}

void MainWindow::onCloseAndDiscardDb()
{
    controller.closeDb();
    ui->stackedWidget->setCurrentWidget(ui->startWidget);
}

void MainWindow::onAddStylesheet()
{
    ui->stylesheetsLayout->addWidget(new StylesheedFileForm());
}

void MainWindow::connectDbStatusControls(DbStatusForm *dbStatusForm)
{
    connect(dbStatusForm, &DbStatusForm::closeAndSave, this, &MainWindow::onCloseAndSaveDb);
    connect(dbStatusForm, &DbStatusForm::closeAndDiscard, this, &MainWindow::onCloseAndDiscardDb);
}

