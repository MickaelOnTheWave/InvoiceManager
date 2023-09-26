/*
 * InvoiceManager
 * Copyright (C) 2023 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    // TODO change isClient to companyType table
    if (!query.exec("CREATE TABLE company (id INTEGER primary key, name TEXT, address TEXT, email TEXT, phone TEXT, "
                    "isClient BOOLEAN, idChild INTEGER)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    // TODO : Move template and stylesheet to a single table with a type column

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
        lastErrorMessage = query.lastError().text();
    return result;
}

bool InvoiceDbController::writeUpdatedCompany(const CompanyData& company, const int parentCompanyId)
{
   QSqlQuery query = createWriteCompanyQuery(company, true);
   const bool result = query.exec();
   if (!result)
   {
       lastErrorMessage = query.lastError().text();
       return false;
   }

   const int newCompanyId = query.lastInsertId().toInt();
   return updateCompanyParenting(parentCompanyId, newCompanyId);
}

std::vector<int> InvoiceDbController::writeInvoiceDetails(const std::vector<InvoiceDetail> &details)
{
    std::vector<int> insertedIds;
    for (const auto& detail : details)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO invoiceelement (description, value) VALUES (:description, :value)");
        query.bindValue(":description", detail.description);
        query.bindValue(":value", detail.value);
        if (query.exec())
            insertedIds.push_back(query.lastInsertId().toInt());
    }
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

bool InvoiceDbController::invoiceExists(const int id) const
{
   QSqlQuery query;
   query.prepare("SELECT id FROM invoice WHERE id = :id LIMIT 1");
   query.bindValue(":id", id);
   const bool result = query.exec();
   return (result && query.next());
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

// TODO : Remove duplication between here and getInvoiceTemplateData()
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
        data.details = createInvoiceDetails(invoiceId);
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
      const QString dateStr = query.value(4).toString();
      const QString notes = query.value(5).toString();
      const QString currency = query.value(6).toString();

      data.date = QDate::fromString(dateStr, dateFormatStr);
      data.notes = notes;
      data.currency = currency;
      data.templatePath = getTemplateFilename(templateId);
      data.stylesheetPath = getStylesheetFilename(stylesheetId);
      data.userCompany = getCompanyData(companyId);
      data.clientCompany = getCompanyData(clientId);
      data.details = createInvoiceDetails(invoiceId);
   }

   return data;
}

std::vector<InvoiceTemplateData> InvoiceDbController::getAllInvoiceTemplateData() const
{
   std::vector<InvoiceTemplateData> data;

   const QString queryStr = "SELECT id, companyId, clientId, templateId, stylesheetId, date, notes, currency "
                            "FROM invoice";
   QSqlQuery query;
   const bool result = query.exec(queryStr);
   if (result)
   {
      while (query.next())
      {
         InvoiceTemplateData invoiceData;
         const int invoiceId = query.value(0).toInt();
         const int companyId = query.value(1).toInt();
         const int clientId = query.value(2).toInt();
         const int templateId = query.value(3).toInt();
         const int stylesheetId = query.value(4).toInt();
         const QString dateStr = query.value(5).toString();
         const QString notes = query.value(6).toString();
         const QString currency = query.value(7).toString();

         invoiceData.id = invoiceId;
         invoiceData.date = QDate::fromString(dateStr, dateFormatStr);
         invoiceData.notes = notes;
         invoiceData.currency = currency;
         invoiceData.templatePath = getTemplateFilename(templateId);
         invoiceData.stylesheetPath = getStylesheetFilename(stylesheetId);
         invoiceData.userCompany = getCompanyData(companyId);
         invoiceData.clientCompany = getCompanyData(clientId);
         invoiceData.details = createInvoiceDetails(invoiceId);
         data.push_back(invoiceData);
      }
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

int InvoiceDbController::getInvoiceCountUsingFile(const int id, const QString& fieldName) const
{
   QSqlQuery query;
   const QString queryStr = "SELECT COUNT(id) FROM invoice WHERE %1 = %2";
   const bool result = query.exec(queryStr.arg(fieldName).arg(id));
   if (result && query.next())
   {
       return query.value(0).toInt();
   }
   return -1;
}

bool InvoiceDbController::removeInvoice(const int id)
{
   const bool result = removeFromInvoiceTable(id);
   if (!result)
      return false;

   removeFromInvoiceElements(id);
   return removeFromInvoiceElements(id);
}

QSqlQuery InvoiceDbController::createWriteCompanyQuery(const CompanyData &data, const bool isClient)
{
    QSqlQuery query;
    query.prepare("INSERT INTO company (name, address, email, phone, isClient, idChild) "
                  "VALUES (:name, :address, :email, :phone, :isClient, :idChild)");
    query.bindValue(":name", data.name);
    query.bindValue(":address", data.address);
    query.bindValue(":email", data.email);
    query.bindValue(":phone", data.phoneNumber);
    query.bindValue(":isClient", isClient);
    query.bindValue(":idChild", -1);
    return query;
}

QSqlDatabase InvoiceDbController::getDatabase()
{
   return db;
}

int InvoiceDbController::getParentCompanyId(const int id)
{
   const QString queryStr = "SELECT id FROM company WHERE idChild = %1";
   QSqlQuery query;
   const bool ok = query.exec(queryStr.arg(id));
   if (ok && query.next())
   {
      return query.value(0).toInt();
   }
   return -1;
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
    const QString dateString = date.toString(dateFormatStr);
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

std::vector<InvoiceDetail> InvoiceDbController::createInvoiceDetails(const int id) const
{
   std::vector<InvoiceDetail> details;

   const std::vector<int> elementsIds = getInvoiceElements(id);
   for (const int elementId : elementsIds)
   {
      const QString queryStr = "SELECT description, value FROM invoiceelement WHERE id = %1";
      QSqlQuery query;
      const bool ok = query.exec(queryStr.arg(elementId));
      if (ok && query.next())
      {
          const QString service = query.value(0).toString();
          const double value = query.value(1).toDouble();
          details.emplace_back(service, value);
      }
   }

   return details;
}

std::vector<int> InvoiceDbController::getInvoiceElements(const int invoiceId) const
{
   std::vector<int> elementsIds;

   const QString queryStr = "SELECT idElement FROM invoicedetailmap WHERE idInvoice = %1";
   QSqlQuery query;
   const bool ok = query.exec(queryStr.arg(invoiceId));
   if (ok)
   {
      while (query.next())
         elementsIds.push_back(query.value(0).toInt());
   }
   return elementsIds;
}

bool InvoiceDbController::removeFromInvoiceTable(const int id)
{
   const QString queryStr = "DELETE FROM invoice WHERE id = %1";
   QSqlQuery query;
   return query.exec(queryStr.arg(id));
}

bool InvoiceDbController::removeFromInvoiceElements(const int id)
{
   const std::vector<int> invoiceElementsIds = getInvoiceElements(id);
   for (const int elementId : invoiceElementsIds)
   {
      const QString queryStr = "DELETE FROM invoiceelement WHERE id = %1";
      QSqlQuery query;
      const bool result = query.exec(queryStr.arg(elementId));
      if (!result)
         return false;
   }
   return true;
}

bool InvoiceDbController::removeFromInvoiceMap(const int id)
{
   const QString queryStr = "DELETE FROM invoicedetailmap WHERE idInvoice = %1";
   QSqlQuery query;
   return query.exec(queryStr.arg(id));
}

bool InvoiceDbController::updateCompanyParenting(const int targetId, const int childId)
{
   const QString queryStr = "UPDATE company SET idChild = %1 WHERE id = %2";
   QSqlQuery query;
   return query.exec(queryStr.arg(childId).arg(targetId));
}
