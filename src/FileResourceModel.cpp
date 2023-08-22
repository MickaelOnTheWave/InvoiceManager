#include "FileResourceModel.h"

FileResourceModel::FileResourceModel(const QString &tableName, QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
    setTable(tableName);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    setHeaderData(0, Qt::Horizontal, tr("Id"));
    setHeaderData(1, Qt::Horizontal, tr("Name"));
    setHeaderData(2, Qt::Horizontal, tr("File"));
}

int FileResourceModel::getId(const int i) const
{
    return data(index(i, 0)).toInt();
}

bool FileResourceModel::insertAtEnd(const QString &name,
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

bool FileResourceModel::remove(const QModelIndex& index)
{
   bool ok = removeRow(index.row());
   if (!ok)
      return false;

   return submitAll();
}
