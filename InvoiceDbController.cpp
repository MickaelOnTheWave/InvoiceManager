#include "InvoiceDbController.h"

#include <QDate>
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

    if (!query.exec("CREATE TABLE template (id INTEGER primary key, name TEXT, file TEXT)"))
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
               "templateId INTEGER, stylesheetId INTEGER, date TEXT, notes TEXT, currency TEXT)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE invoiceelement (id INTEGER primary key, description TEXT, value DOUBLE)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE invoicedetailmap (id INTEGER primary key, idInvoice INTEGER, idElement INTEGER)"))
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

std::vector<int> InvoiceDbController::writeInvoiceDetails(const std::vector<InvoiceDetail> &details)
{
    std::vector<int> insertedIds;
    // Fail safe behaviour : insert elements one by one.
    // Once we have an answer from StackOverflow we can do the batch insert.

    for (const auto& detail : details)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO invoiceelement (description, value) VALUES (:description, :value)");
        query.bindValue(":description", detail.description);
        query.bindValue(":value", detail.value);
        if (query.exec())
            insertedIds.push_back(query.lastInsertId().toInt());
    }



/*    QSqlQuery query(db);
    const bool result = query.exec(createInvoiceDetailsWriteQuery(details));

    std::vector<int> insertedIds;
    if (result)
    {
        while (query.next())
            insertedIds.push_back(query.value(0).toInt());
    }*/
    return insertedIds;
}

bool InvoiceDbController::writeInvoice(const int invoiceId, const int clientId, const int templateId,
                                       const int stylesheetId, const std::vector<int> &detailsIds,
                                       const QDate &date, const QString& notes, const QString& currency)
{
    const bool ok = writeToInvoiceTable(invoiceId, clientId, templateId, stylesheetId, date, notes, currency);
    if (ok)
        return writeToInvoiceMapTable(invoiceId, detailsIds);
    return false;
}

QString InvoiceDbController::getUserCompanyName() const
{
    QSqlQuery query(db);
    const bool result = query.exec(createUserCompanyRequest("name"));
    if (result && query.next())
    {
        return query.value(0).toString();
    }

    return query.lastError().text();
}

int InvoiceDbController::getUserCompanyId() const
{
    QSqlQuery query(db);
    const bool result = query.exec(createUserCompanyRequest("id"));
    if (result && query.next())
        return query.value(0).toInt();
    return -1;
}

CompanyData InvoiceDbController::getUserCompanyData() const
{
    CompanyData data;

    QString queryStr = "SELECT name, email, phone, address FROM company WHERE isClient = FALSE ORDER BY id DESC LIMIT 1";
    QSqlQuery query(db);
    const bool result = query.exec(queryStr);
    if (result && query.next())
    {
        data.name = query.value(0).toString();
        data.email = query.value(1).toString();
        data.phoneNumber = query.value(2).toString();
        data.address = query.value(3).toString();
    }
    return data;
}

int InvoiceDbController::getLastUsedInvoiceId() const
{
    QSqlQuery query;
    const bool ok = query.exec("SELECT id FROM invoice ORDER BY id DESC LIMIT 1");
    if (ok && query.next())
        return query.value(0).toInt();
    return -1;
}

InvoiceData InvoiceDbController::getLastInvoiceData() const
{
    InvoiceData data;
    QSqlQuery query;
    const bool ok = query.exec("SELECT id, clientId, templateId, stylesheetId, notes, currency FROM invoice ORDER BY id DESC LIMIT 1");
    if (ok && query.next())
    {

        data.clientId = query.value(1).toInt();
        data.templateId = query.value(2).toInt();
        data.stylesheetId = query.value(3).toInt();
        data.notes = query.value(4).toString();
        data.currency = query.value(5).toString();

        const int invoiceId = query.value(0).toInt();
        const QString queryStr = "SELECT idElement FROM invoicedetailmap WHERE idInvoice = %1";
        const bool ok = query.exec(queryStr.arg(invoiceId));
        if (ok)
        {
            while (query.next())
            {
                const int elementId = query.value(0).toInt();
                const QString queryStr = "SELECT description, value FROM invoiceelement WHERE id = %1";
                const bool ok = query.exec(queryStr.arg(elementId));
                if (ok && query.next())
                {
                    const QString service = query.value(0).toString();
                    const double value = query.value(1).toDouble();
                    data.details.emplace_back(service, value);
                }
            }
        }
    }
    return data;
}

InvoiceTemplateData InvoiceDbController::getInvoiceTemplateData(const int invoiceId) const
{
   InvoiceTemplateData data;
   data.id = invoiceId;

   const QString queryStr = "SELECT companyId, clientId, templateId, stylesheetId, date, notes, currency "
                            "FROM invoice WHERE id = %1";
   QSqlQuery query(queryStr.arg(invoiceId));
   if (query.first())
   {
      const int companyId = query.value(0).toInt();
      const int clientId = query.value(1).toInt();
      const int templateId = query.value(2).toInt();
      const int stylesheetId = query.value(3).toInt();
      const QDate date = query.value(4).toDate();
      const QString notes = query.value(5).toString();
      const QString currency = query.value(6).toString();

      data.date = date;
      data.notes = notes;
      data.currency = currency;
      data.templatePath = getTemplateFilename(templateId);
      data.stylesheetPath = getStylesheetFilename(stylesheetId);
      data.userCompany = getCompanyData(companyId);
      data.clientCompany = getCompanyData(clientId);
   }

   return data;
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

QString InvoiceDbController::getTemplateFilename(const int id) const
{
    return getFilenameFromId("template", id);
}

QString InvoiceDbController::getStylesheetFilename(const int id) const
{
    return getFilenameFromId("stylesheet", id);
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

QString InvoiceDbController::createUserCompanyRequest(const QString &field)
{
    return QString("SELECT %1 FROM company WHERE isClient = FALSE ORDER BY id DESC LIMIT 1").arg(field);
}

QString InvoiceDbController::createInvoiceDetailsWriteQuery(const std::vector<InvoiceDetail> &details) const
{
    QString queryContent = "INSERT INTO invoiceelement (description, value) VALUES";
    bool insertedFirstEntry = false;
    for (const auto& detail : details)
    {
        if (insertedFirstEntry)
            queryContent += ",";
        else
            insertedFirstEntry = true;

        queryContent += QString(" ('%1', '%2')").arg(detail.description).arg(detail.value);
    }
    return queryContent;
}

int InvoiceDbController::writeToInvoiceTable(const int invoiceId, const int clientId, const int templateId,
                                             const int stylesheetId, const QDate &date, const QString& notes,
                                             const QString& currency)
{
    const QString dateString = date.toString("d MMM yyyy");
    QSqlQuery query;
    query.prepare("INSERT INTO invoice (id, companyId, clientId, templateId, stylesheetId, date, notes, currency)"
                  "VALUES (:id, :companyId, :clientId, :templateId, :stylesheetId, :date, :notes, :currency)");
    query.bindValue(":id", invoiceId);
    query.bindValue(":companyId", getUserCompanyId());
    query.bindValue(":clientId", clientId);
    query.bindValue(":templateId", templateId);
    query.bindValue(":stylesheetId", stylesheetId);
    query.bindValue(":date", dateString);
    query.bindValue(":notes", notes);
    query.bindValue(":currency", currency);
    if (query.exec())
    {
        return query.lastInsertId().toInt();
    }
    return -1;
}

bool InvoiceDbController::writeToInvoiceMapTable(const int invoiceId, const std::vector<int> &detailsIds)
{
    for (const int detailId: detailsIds)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO invoicedetailmap (idInvoice, idElement) VALUES (:invoice, :detail)");
        query.bindValue(":invoice", invoiceId);
        query.bindValue(":detail", detailId);
        if (!query.exec())
            return false;
    }
    return true;
}

QString InvoiceDbController::getFilenameFromId(const QString &table, const int id) const
{
    QSqlQuery query;
    query.prepare(QString("SELECT file FROM %1 WHERE id = :id").arg(table));
    query.bindValue(":id", id);
    const bool ok = query.exec();
    if (ok && query.next())
        return query.value(0).toString();
    return QString();
}

CompanyData InvoiceDbController::getCompanyData(const int id) const
{
   CompanyData data;

   QString queryStr = "SELECT name, address, email, phone FROM company WHERE id = %1";
   QSqlQuery query(queryStr.arg(id));
   if (query.first())
   {
       data.name = query.value(0).toString();
       data.address = query.value(1).toString();
       data.email = query.value(2).toString();
       data.phoneNumber = query.value(3).toString();
   }
   return data;
}
