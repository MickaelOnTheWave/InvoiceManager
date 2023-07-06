#include "MorePage.h"
#include "ui_MorePage.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);

    connect(ui->backButton, &QAbstractButton::clicked, this, &MorePage::back);
}

MorePage::~MorePage()
{
    delete ui;
}

void MorePage::connectViewsToModels(QAbstractItemModel *clientModel,
                                    QAbstractItemModel *stylesheetModel)
{
    ui->clientsView->setModel(clientModel);
    ui->stylesheetsView->setModel(stylesheetModel);

    ui->clientsView->show();
}
