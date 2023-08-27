#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QSqlQueryModel>

#include "CompanyData.h"

class ClientModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ClientModel(QObject *parent = nullptr);

    int getId(const int i) const;
    CompanyData getDataAtRow(const int i) const;

    bool insertAtEnd(const CompanyData& data);
    bool remove(const QModelIndex& i);

    void refreshModel();
};

#endif // CLIENTMODEL_H
