#ifndef INVOICEVIEWDIALOG_H
#define INVOICEVIEWDIALOG_H

#include <QDialog>

#include "src/InvoiceData.h"

namespace Ui {
class InvoiceViewDialog;
}

class InvoiceViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InvoiceViewDialog(QWidget *parent = nullptr);
    virtual ~InvoiceViewDialog();

    void setData(const InvoiceTemplateData& data);

private:
    Ui::InvoiceViewDialog *ui;
};

#endif // INVOICEVIEWDIALOG_H
