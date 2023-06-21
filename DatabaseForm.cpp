#include "DatabaseForm.h"
#include "ui_DatabaseForm.h"

DatabaseForm::DatabaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseForm)
{
    ui->setupUi(this);
}

DatabaseForm::~DatabaseForm()
{
    delete ui;
}
