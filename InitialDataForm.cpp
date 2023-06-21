#include "InitialDataForm.h"
#include "ui_InitialDataForm.h"

InitialDataForm::InitialDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InitialDataForm)
{
    ui->setupUi(this);

    connect(ui->addButton, &QAbstractButton::clicked, this, &InitialDataForm::onAddStylesheet);

    connect(ui->confirmButton, &QAbstractButton::clicked, this, &InitialDataForm::confirm);
    connect(ui->cancelButton, &QAbstractButton::clicked, this, &InitialDataForm::cancel);
}

InitialDataForm::~InitialDataForm()
{
    delete ui;
}

void InitialDataForm::onAddStylesheet()
{

}
