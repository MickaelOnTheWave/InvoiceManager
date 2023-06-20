#include "InvoiceDbController.h"

#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>

InvoiceDbController::InvoiceDbController()
{

}

void InvoiceDbController::createDb(const QString &filename)
{
    dbFilename = filename;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
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

void InvoiceDbController::save()
{

}

void InvoiceDbController::closeDb()
{

}
