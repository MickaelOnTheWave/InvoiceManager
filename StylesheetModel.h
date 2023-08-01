#ifndef STYLESHEETMODEL_H
#define STYLESHEETMODEL_H

#include <QSqlTableModel>

class StylesheetModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit StylesheetModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());

    int getId(const int i) const;

    bool insertAtEnd(const QString& name, const QString &path);

private:
};

#endif // STYLESHEETMODEL_H
