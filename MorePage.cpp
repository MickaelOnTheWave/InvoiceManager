#include "MorePage.h"
#include "ui_MorePage.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);
}

MorePage::~MorePage()
{
    delete ui;
}
