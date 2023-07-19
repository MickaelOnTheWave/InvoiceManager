#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QSqlQueryModel>

#include "Company.h"

class ClientModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ClientModel(QObject *parent = nullptr);

    CompanyData getDataAtRow(const int i) const;

    bool insertAtEnd(const CompanyData& data);

private:
    void refreshModel();
};

#endif // CLIENTMODEL_H
