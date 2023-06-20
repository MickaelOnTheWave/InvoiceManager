#include "DbStatusForm.h"
#include "ui_DbStatusForm.h"

#include <QPushButton>

DbStatusForm::DbStatusForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DbStatusForm)
{
    ui->setupUi(this);

    connect(ui->closeButton, &QPushButton::clicked, this, &DbStatusForm::onClosePushed);
    connect(ui->closeAndSaveButton, &QPushButton::clicked, this, &DbStatusForm::onCloseAndSave);
    connect(ui->closeAndDiscardButton, &QPushButton::clicked, this, &DbStatusForm::onCloseAndDiscard);
    connect(ui->dontCloseButton, &QPushButton::clicked, this, &DbStatusForm::onDontClose);
}

DbStatusForm::~DbStatusForm()
{
    delete ui;
}

void DbStatusForm::setDbFile(const QString &dbFile)
{
    ui->dbLabel->setText(dbFile);
}

void DbStatusForm::onClosePushed()
{
    ui->dbActionsWidget->setCurrentWidget(ui->confirmPage);
}

void DbStatusForm::onCloseAndSave()
{
    ui->dbActionsWidget->setCurrentWidget(ui->openedPage);
    emit closeAndSave();
}

void DbStatusForm::onCloseAndDiscard()
{
    ui->dbActionsWidget->setCurrentWidget(ui->openedPage);
    emit closeAndDiscard();
}

void DbStatusForm::onDontClose()
{
    ui->dbActionsWidget->setCurrentWidget(ui->openedPage);
}
