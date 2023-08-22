#ifndef FileResourceModel_H
#define FileResourceModel_H

#include <QSqlTableModel>

class FileResourceModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit FileResourceModel(const QString& tableName,
                               QObject *parent = nullptr,
                               QSqlDatabase db = QSqlDatabase());

    int getId(const int i) const;

    bool insertAtEnd(const QString& name, const QString &path);
    bool remove(const QModelIndex& index);

private:
};

#endif // FileResourceModel_H
