#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QSqlTableModel>

#include "Company.h"

class ClientModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit ClientModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    CompanyData getDataAtRow(const int i) const;

    bool insertAtEnd(const CompanyData& data);

private:
};

#endif // CLIENTMODEL_H
