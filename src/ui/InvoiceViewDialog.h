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

signals:
    void deleteConfirmed(const int id);

private slots:
    void onDeleteClicked();

private:
    Ui::InvoiceViewDialog *ui;
    InvoiceTemplateData invoiceData;
};

#endif // INVOICEVIEWDIALOG_H
