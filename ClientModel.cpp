#include "ClientModel.h"

#include <QSqlQuery>
#include "InvoiceDbController.h"

#include <QSqlField>
#include <QSqlRecord>

ClientModel::ClientModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    refreshModel();
}

int ClientModel::getId(const int i) const
{
    return data(index(i, 0)).toInt();
}

CompanyData ClientModel::getDataAtRow(const int i) const
{
    CompanyData companyData;
    companyData.name = data(index(i, 1)).toString();
    companyData.address = data(index(i, 2)).toString();
    companyData.email = data(index(i, 3)).toString();
    companyData.phoneNumber = data(index(i, 4)).toString();
    return companyData;
}

bool ClientModel::insertAtEnd(const CompanyData &data)
{
    const int rowIndex = rowCount();

    QSqlQuery query = InvoiceDbController::createWriteCompanyQuery(data, true);

    const bool result = query.exec();
    if (!result)
        return false;

    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    endInsertRows();

    refreshModel();
    return result;
}

void ClientModel::refreshModel()
{
    setQuery("SELECT * FROM company WHERE isClient = TRUE");
    setHeaderData(0, Qt::Horizontal, tr("Id"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("Address"));
    setHeaderData(3, Qt::Horizontal, tr("Email"));
}
