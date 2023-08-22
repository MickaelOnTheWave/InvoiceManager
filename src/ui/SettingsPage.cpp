#include "SettingsPage.h"
#include "ui_SettingsPage.h"

#include <QDialogButtonBox>
#include <QSettings>

SettingsPage::SettingsPage(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::SettingsPage)
{
   ui->setupUi(this);

   connect(ui->resetButton, &QPushButton::clicked, this, &SettingsPage::onReset);
   connect(ui->quitBox, &QDialogButtonBox::accepted, this, &SettingsPage::onSave);
   connect(ui->quitBox->button(QDialogButtonBox::Discard), &QAbstractButton::clicked,
           this, &SettingsPage::done);

   loadSettings();
}

SettingsPage::~SettingsPage()
{
   delete ui;
}

void SettingsPage::refresh()
{
   loadSettings();
}

void SettingsPage::onReset()
{
   ui->removeConfirmBox->setChecked(true);
}

void SettingsPage::onSave()
{
   saveSettings();
   emit done();
}

void SettingsPage::loadSettings()
{
   QSettings settings;
   const bool removeConfirm = settings.value("user/removeconfirmation", true).toBool();

   ui->removeConfirmBox->setChecked(removeConfirm);
}

void SettingsPage::saveSettings()
{
   QSettings settings;
   settings.setValue("user/removeconfirmation", ui->removeConfirmBox->isChecked());
}
