#include "InvoiceDbController.h"

#include <QtSql/QSqlQuery>
#include <qsqlerror.h>

InvoiceDbController::InvoiceDbController()
{
}

void InvoiceDbController::createDb(const QString &filename)
{
    dbFilename = filename;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    db.open();

    QSqlQuery query;
    query.exec("CREATE TABLE client (id INTEGER primary key, name TEXT, address TEXT)");
    query.exec("CREATE TABLE company (id INTEGER primary key, name TEXT, address TEXT, email TEXT)");
    query.exec("CREATE TABLE stylesheet (id INTEGER primary key, file TEXT)");
    query.exec("CREATE TABLE invoice (id INTEGER primary key, companyId INTEGER, clientId INTEGER, "
               "stylesheetId INTEGER, value DOUBLE, date TEXT)");
}

void InvoiceDbController::openDb(const QString &filename)
{
    dbFilename = filename;
}

void InvoiceDbController::closeDb()
{

}

void InvoiceDbController::write(const QString &companyName, QStringListModel *stylesheetsModel)
{
    QSqlQuery query;
    query.exec("INSERT INTO company (name) VALUES ('" + companyName + "')");

    /*
    for ()
    {
        stylesheetsModel->data(index);
        query.exec("INSERT INTO stylesheet (file) VALUES ('" + stylesheet + "')");
    }*/
}

QString InvoiceDbController::getCompanyName() const
{
    QSqlQuery query(db);
    const bool result = query.exec("SELECT name FROM company ORDER BY id DESC LIMIT 1");
    if (!result)
    {
        QSqlError error = query.lastError();
        auto errorDesc = error.text();
        int lala = 3;
    }

    auto jkjk = query.value(0);
    return query.value(0).toString();
}
