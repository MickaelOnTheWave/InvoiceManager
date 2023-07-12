#include "ClientModel.h"

ClientModel::ClientModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    setTable("client");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    bool ok = setHeaderData(0, Qt::Horizontal, tr("ID"));
    ok = setHeaderData(1, Qt::Horizontal, tr("Name"));
    ok = setHeaderData(2, Qt::Horizontal, tr("Address"));
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
