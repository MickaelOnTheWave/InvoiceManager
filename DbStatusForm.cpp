#include "DbStatusForm.h"
#include "ui_DbStatusForm.h"

#include <QPushButton>

DbStatusForm::DbStatusForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DbStatusForm)
{
    ui->setupUi(this);
}

DbStatusForm::~DbStatusForm()
{
    delete ui;
}

void DbStatusForm::setData(const QString &dbFile, const int dbVersion)
{
    ui->dbFileLabel->setText(dbFile);
    ui->dbVersionLabel->setText(QString::number(dbVersion));
}
