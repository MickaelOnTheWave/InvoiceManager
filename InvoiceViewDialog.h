#ifndef INVOICEVIEWDIALOG_H
#define INVOICEVIEWDIALOG_H

#include <QDialog>

#include "InvoiceData.h"

namespace Ui {
class InvoiceViewDialog;
}

class InvoiceViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InvoiceViewDialog(const int _invoiceId, QWidget *parent = nullptr);
    ~InvoiceViewDialog();

    void setData(const InvoiceTemplateData& data);

private:
    Ui::InvoiceViewDialog *ui;

    const int invoiceId;
};

#endif // INVOICEVIEWDIALOG_H
