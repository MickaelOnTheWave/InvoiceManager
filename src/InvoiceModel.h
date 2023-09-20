#ifndef INVOICEMODEL_H
#define INVOICEMODEL_H

#include <QSqlQueryModel>

class InvoiceModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit InvoiceModel(QObject *parent = nullptr);

   QVariant data(const QModelIndex& item, int role = Qt::DisplayRole) const override;

    void refresh();
};

#endif // INVOICEMODEL_H
