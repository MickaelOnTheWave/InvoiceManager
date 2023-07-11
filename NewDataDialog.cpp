#include "NewDataDialog.h"
#include "ui_NewDataDialog.h"

NewDataDialog::NewDataDialog(QWidget* contentWidget, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDataDialog)
{
    ui->setupUi(this);

    layout()->replaceWidget(ui->replacedWidget, contentWidget);
}

NewDataDialog::~NewDataDialog()
{
    delete ui;
}
