#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QSqlQueryModel>

#include "InvoiceModule/include/CompanyData.h"
#include "InvoiceModule/include/InvoiceDbController.h"

class ClientModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ClientModel(InvoiceDbController* _controller,
                         QObject *parent = nullptr);

    int getId(const int i) const;
    CompanyData getDataAtRow(const int i) const;

    bool insertAtEnd(const CompanyData& data);
    bool remove(const QModelIndex& i);

    void refreshModel();

private:
    InvoiceDbController* controller;
};

#endif // CLIENTMODEL_H
