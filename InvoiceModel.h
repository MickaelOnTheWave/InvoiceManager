#ifndef INVOICEMODEL_H
#define INVOICEMODEL_H

#include <QSqlQueryModel>

class InvoiceModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit InvoiceModel(QObject *parent = nullptr);

    void refresh();
};

#endif // INVOICEMODEL_H
