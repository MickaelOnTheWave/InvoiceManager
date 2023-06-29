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
    if (!query.exec("CREATE TABLE client (id INTEGER primary key, name TEXT, address TEXT)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE company (id INTEGER primary key, name TEXT, address TEXT, email TEXT)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE stylesheet (id INTEGER primary key, file TEXT)"))
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

    return true;
}

void InvoiceDbController::openDb(const QString &filename)
{
    createDbConnection(filename);
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
    QSqlQuery query;
    query.prepare("INSERT INTO company (name, address, email) VALUES (:name, :address, :email)");
    query.bindValue(":name", company.name);
    query.bindValue(":address", company.address);
    query.bindValue(":email", company.email);
    const bool result = query.exec();
    if (!result)
    {
        lastErrorMessage = query.lastError().text();
    }
    return result;
}

bool InvoiceDbController::writeStylesheets(const QStringList& stylesheets)
{
    bool result = true;
    for (const auto& stylesheet : stylesheets)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO stylesheet (file) VALUES (:file)");
        query.bindValue(":file", stylesheet);
        result = query.exec();
        if (!result)
        {
            lastErrorMessage = query.lastError().text();
            break;
        }
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

void InvoiceDbController::createDbConnection(const QString &filename)
{
    dbFilename = filename;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    db.open();
}
