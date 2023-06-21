#include "DatabaseForm.h"
#include "ui_DatabaseForm.h"

DatabaseForm::DatabaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseForm)
{
    ui->setupUi(this);
    connect(ui->dbStatusWidget, &DbStatusForm::closeAndSave, this, &DatabaseForm::closeAndSave);
    connect(ui->dbStatusWidget, &DbStatusForm::closeAndDiscard, this, &DatabaseForm::closeAndDiscard);
}

DatabaseForm::~DatabaseForm()
{
    delete ui;
}
