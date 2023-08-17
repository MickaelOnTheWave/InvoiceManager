#include "SettingsPage.h"
#include "ui_SettingsPage.h"

#include <QDialogButtonBox>

SettingsPage::SettingsPage(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::SettingsPage)
{
   ui->setupUi(this);

   connect(ui->resetButton, &QPushButton::clicked, this, &SettingsPage::onReset);
   connect(ui->quitBox, &QDialogButtonBox::accepted, this, &SettingsPage::onSave);
   connect(ui->quitBox->button(QDialogButtonBox::Discard), &QAbstractButton::clicked,
           this, &SettingsPage::done);
}

SettingsPage::~SettingsPage()
{
   delete ui;
}

void SettingsPage::onReset()
{

}

void SettingsPage::onSave()
{
   // TODO : save to preferences
   emit done();
}
