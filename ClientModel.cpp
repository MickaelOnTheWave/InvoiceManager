#include "ClientModel.h"

#include <QSqlQuery>

ClientModel::ClientModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    setQuery("SELECT name, address, email FROM company ");

    setHeaderData(0, Qt::Horizontal, tr("Name"));
    setHeaderData(1, Qt::Horizontal, tr("Address"));
    setHeaderData(2, Qt::Horizontal, tr("Email"));
}

CompanyData ClientModel::getDataAtRow(const int i) const
{
    CompanyData companyData;
    companyData.name = data(index(i, 0)).toString();
    companyData.address = data(index(i, 1)).toString();
    companyData.email = data(index(i, 2)).toString();
    return companyData;
}

bool ClientModel::insertAtEnd(const CompanyData &data)
{
    const int rowIndex = rowCount();

    QSqlQuery query;
    query.prepare("INSERT INTO company (name, address, email) VALUES (:name, :address, :email)");
    query.bindValue(":name", data.name);
    query.bindValue(":address", data.address);
    query.bindValue(":email", data.email);
    const bool result = query.exec();
    if (result)
    {
        // TODO : either implement setData or find a way to force a refresh.
        // At the moment, this is not working.
        refreshModel();
        emit dataChanged(index(rowIndex, 0), index(rowIndex, 2));
    }
    return result;
/*
    const bool ok = insertRow(rowIndex);
    if (!ok)
        return false;

    setData(index(rowIndex, 0), data.name);
    setData(index(rowIndex, 1), data.address);
    setData(index(rowIndex, 2), data.email);
    return true;*/
}

void ClientModel::refreshModel()
{
    setQuery(query());
}
