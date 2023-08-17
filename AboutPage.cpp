#include "AboutPage.h"
#include "ui_AboutPage.h"

AboutPage::AboutPage(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::AboutPage)
{
   ui->setupUi(this);

   connect(ui->backButton, &QAbstractButton::clicked, this, &AboutPage::backClicked);
}

AboutPage::~AboutPage()
{
   delete ui;
}

void AboutPage::setVersions(const QString& appVersion, const int dbVersion)
{
   ui->appVersionLabel->setText(appVersion);
   ui->dbVersionLabel->setText(QString::number(dbVersion));
}
