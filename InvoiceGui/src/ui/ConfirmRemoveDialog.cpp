#include "ConfirmRemoveDialog.h"
#include "ui_ConfirmRemoveDialog.h"

#include <QSettings>

bool ConfirmRemoveDialog::ask(const QString &message, const QString &settingsId)
{
    QSettings settings;
    const bool confirmRemoval = settings.value(settingsId, true).toBool();
    if (!confirmRemoval)
        return true;

    ConfirmRemoveDialog removeDialog(message, settingsId);
    const int selectedOption = removeDialog.exec();
    if (selectedOption == QDialog::Accepted)
    {
        const bool needsConfirmation = !removeDialog.ui->dontAskCheckbox->isChecked();
        settings.setValue(settingsId, needsConfirmation);
        return true;
    }
    return false;
}

ConfirmRemoveDialog::ConfirmRemoveDialog(const QString &message, const QString &settingsId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmRemoveDialog)
{
    ui->setupUi(this);
    ui->message->setText(message);
}

ConfirmRemoveDialog::~ConfirmRemoveDialog()
{
    delete ui;
}
