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
    static const int currentDbVersion = 4;

    InvoiceDbController();

    bool createDb(const QString& filename);
    bool openDb(const QString& filename);
    void closeDb();

    QString getLastError() const;

    bool writeUserCompany(const CompanyData& company);

    QString getCompanyName() const;
    QString getDatabaseFile() const;
    int getDatabaseVersion() const;

    QSqlDatabase getDatabase();

    static QSqlQuery createWriteCompanyQuery(const CompanyData& data, const bool isClient);

private:
    bool createDbConnection(const QString& filename);

    QString dbFilename;
    QSqlDatabase db; // TODO : remove this. See tip from Qt documentqtion.
    QString lastErrorMessage;
};

#endif // INVOICEDBCONTROLLER_H
