#include "NewInvoicePage.h"
#include "ui_NewInvoicePage.h"

#include <QDialogButtonBox>

NewInvoicePage::NewInvoicePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewInvoicePage)
{
    ui->setupUi(this);

    connect(ui->closeBox, &QDialogButtonBox::accepted, this, &NewInvoicePage::create);
    connect(ui->closeBox, &QDialogButtonBox::rejected, this, &NewInvoicePage::cancel);
}

NewInvoicePage::~NewInvoicePage()
{
    delete ui;
}
