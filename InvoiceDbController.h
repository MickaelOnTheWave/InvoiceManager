#ifndef INVOICEDBCONTROLLER_H
#define INVOICEDBCONTROLLER_H

#include <QtSql/qsqldatabase.h>
#include <QStringListModel>
#include <QObject>
#include <QString>

#include "Company.h"

class InvoiceDbController : public QObject
{
    Q_OBJECT

public:
    InvoiceDbController();

    bool createDb(const QString& filename);
    void openDb(const QString& filename);
    void closeDb();

    QString getLastError() const;

    bool writeUserCompany(const Company& company);
    bool writeStylesheets(const QStringList &stylesheets);

    QString getCompanyName() const;

private:
    void createDbConnection(const QString& filename);

    QString dbFilename;
    QSqlDatabase db; // TODO : remove this. See tip from Qt documentqtion.
    QString lastErrorMessage;
};

#endif // INVOICEDBCONTROLLER_H
