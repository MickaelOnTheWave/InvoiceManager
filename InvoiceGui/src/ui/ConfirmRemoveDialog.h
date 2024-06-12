#ifndef CONFIRMREMOVEDIALOG_H
#define CONFIRMREMOVEDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmRemoveDialog;
}

class ConfirmRemoveDialog : public QDialog
{
    Q_OBJECT
public:
    static bool ask(const QString& message, const QString& settingsId);

private:
    explicit ConfirmRemoveDialog(const QString& message,
                                 const QString& settingsId,
                                 QWidget *parent = nullptr);
    ~ConfirmRemoveDialog();

private:
    Ui::ConfirmRemoveDialog *ui;
};

#endif // CONFIRMREMOVEDIALOG_H
