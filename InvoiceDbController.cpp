#include "InvoiceDbController.h"

#include <QFile>
#include <QtSql/QSqlQuery>
#include <qsqlerror.h>

InvoiceDbController::InvoiceDbController()
{
}

bool InvoiceDbController::createDb(const QString &filename)
{
    QFile file(filename);
    if (file.exists())
    {
        const bool result = file.remove();
        if (!result)
        {
            lastErrorMessage = "Can't remove previous file";
            return false;
        }
    }

    createDbConnection(filename);

    QSqlQuery query;
    if (!query.exec("CREATE TABLE company (id INTEGER primary key, name TEXT, address TEXT, email TEXT, phone TEXT, isClient BOOLEAN)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE stylesheet (id INTEGER primary key, name TEXT, file TEXT)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE invoice (id INTEGER primary key, companyId INTEGER, clientId INTEGER, "
               "stylesheetId INTEGER, value DOUBLE, date TEXT)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE version (value INTEGER)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    query.prepare("INSERT INTO version (value) VALUES (:currentVersion)");
    query.bindValue(":currentVersion", currentDbVersion);
    if (!query.exec())
    {

        lastErrorMessage = query.lastError().text();
        return false;
    }

    return true;
}

bool InvoiceDbController::openDb(const QString &filename)
{
    return createDbConnection(filename);
}

void InvoiceDbController::closeDb()
{
    db.close();
}

QString InvoiceDbController::getLastError() const
{
    return lastErrorMessage;
}

bool InvoiceDbController::writeUserCompany(const CompanyData &company)
{
    QSqlQuery query = createWriteCompanyQuery(company, false);
    const bool result = query.exec();
    if (!result)
    {
        lastErrorMessage = query.lastError().text();
    }
    return result;
}

QString InvoiceDbController::getCompanyName() const
{
    QSqlQuery query(db);
    const bool result = query.exec("SELECT name FROM company ORDER BY id DESC LIMIT 1");
    if (result && query.next())
    {
        return query.value(0).toString();
    }

    return query.lastError().text();
}

QString InvoiceDbController::getDatabaseFile() const
{
    return dbFilename;
}

int InvoiceDbController::getDatabaseVersion() const
{
    QSqlQuery query(db);
    const bool result = query.exec("SELECT value FROM version");
    if (result && query.next())
    {
        return query.value(0).toInt();
    }

    return -1;
}

QSqlQuery InvoiceDbController::createWriteCompanyQuery(const CompanyData &data, const bool isClient)
{
    QSqlQuery query;
    query.prepare("INSERT INTO company (name, address, email, phone, isClient) "
                  "VALUES (:name, :address, :email, :phone, :isClient)");
    query.bindValue(":name", data.name);
    query.bindValue(":address", data.address);
    query.bindValue(":email", data.email);
    query.bindValue(":phone", data.phoneNumber);
    query.bindValue(":isClient", isClient);
    return query;
}

QSqlDatabase InvoiceDbController::getDatabase()
{
    return db;
}

bool InvoiceDbController::createDbConnection(const QString &filename)
{
    dbFilename = filename;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    return db.open();
}
