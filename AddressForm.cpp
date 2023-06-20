#include "AddressForm.h"
#include "ui_AddressForm.h"

AddressForm::AddressForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddressForm)
{
    ui->setupUi(this);
}

AddressForm::~AddressForm()
{
    delete ui;
}
