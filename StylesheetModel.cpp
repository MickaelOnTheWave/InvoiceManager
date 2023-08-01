#include "StylesheetModel.h"

StylesheetModel::StylesheetModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    setTable("stylesheet");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    setHeaderData(0, Qt::Horizontal, tr("Id"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("File"));
}

int StylesheetModel::getId(const int i) const
{
    return data(index(i, 0)).toInt();
}

bool StylesheetModel::insertAtEnd(const QString &name,
                                  const QString &path)
{
    const int rowIndex = rowCount();
    const bool ok = insertRow(rowIndex);
    if (!ok)
        return false;

    setData(index(rowIndex, 1), name);
    setData(index(rowIndex, 2), path);
    return submitAll();
}
