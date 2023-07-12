#include "ClientModel.h"

ClientModel::ClientModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    setTable("client");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("Address"));
}

CompanyData ClientModel::getDataAtRow(const int i) const
{
    CompanyData companyData;
    companyData.name = data(index(i, 1)).toString();
    companyData.address = data(index(i, 2)).toString();
    return companyData;
}

bool ClientModel::insertAtEnd(const CompanyData &data)
{
    const int rowIndex = rowCount();
    const bool ok = insertRow(rowIndex);
    if (!ok)
        return false;

    setData(index(rowIndex, 1), data.name);
    setData(index(rowIndex, 2), data.address);
    return submitAll();
}
