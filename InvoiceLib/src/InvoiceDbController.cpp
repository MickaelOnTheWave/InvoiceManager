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

#include <algorithm>

using namespace std;

double IncomeHistory::getMaxValue() const
{
   std::vector<double> totalsByDateSpan;
   for (int i=0; i<data.front().second.size(); ++i)
      totalsByDateSpan.push_back(0.0);

   for(const auto& clientIncome : data)
   {
      auto itTotals = totalsByDateSpan.begin();
      for (const auto& incomeData : clientIncome.second)
      {
         *itTotals += incomeData;
         ++itTotals;
      }
   }

   auto itMaxElement = max_element(totalsByDateSpan.begin(), totalsByDateSpan.end());
   return (itMaxElement != totalsByDateSpan.end()) ? *itMaxElement : 0.0;
}

void IncomeHistory::push_back(const ClientIncomeHistory& newData)
{
   data.push_back(newData);
}

vector<IncomeHistory::ClientIncomeHistory>::iterator IncomeHistory::begin()
{
   return data.begin();
}

vector<IncomeHistory::ClientIncomeHistory>::iterator IncomeHistory::end()
{
   return data.end();
}

vector<IncomeHistory::ClientIncomeHistory>::const_iterator IncomeHistory::begin() const
{
   return data.begin();
}

vector<IncomeHistory::ClientIncomeHistory>::const_iterator IncomeHistory::end() const
{
   return data.end();
}



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

    if (!query.exec("CREATE TABLE fileresource (id INTEGER primary key, name TEXT, content TEXT, typeId INTEGER)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    createResourceTypeData(&lastErrorMessage);

    if (!query.exec("CREATE TABLE invoice (id INTEGER primary key, companyId INTEGER, clientId INTEGER, "
               "templateId INTEGER, stylesheetId INTEGER, date TEXT, notes TEXT, currency TEXT)"))
    {
        lastErrorMessage = query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE TABLE invoiceelement (id INTEGER primary key, description TEXT, value DOUBLE, quantity DOUBLE)"))
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
        query.prepare("INSERT INTO invoiceelement (description, value, quantity) VALUES (:description, :value, :quantity)");
        query.bindValue(":description", detail.description);
        query.bindValue(":value", detail.value);
        query.bindValue(":quantity", detail.quantity);
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

bool InvoiceDbController::writeInvoice(const InvoiceDbData& data)
{
   return writeInvoice(data.id, data.clientId, data.templateId, data.stylesheetId,
                       data.detailsIds, data.date, data.notes, data.currency);
}

bool InvoiceDbController::invoiceExists(const int id) const
{
   QSqlQuery query;
   query.prepare("SELECT id FROM invoice WHERE id = :id LIMIT 1");
   query.bindValue(":id", id);
   const bool result = query.exec();
   return (result && query.next());
}

InvoiceUserData InvoiceDbController::toUserData(const InvoiceDbData& dbData) const
{
   InvoiceUserData userData(dbData);
   userData.templateData = getTemplateData(dbData.templateId);
   userData.stylesheetData = getStylesheetData(dbData.stylesheetId);
   userData.userCompany = getCompanyData(dbData.clientId);
   userData.clientCompany = getCompanyData(dbData.clientId);

   for (const int detailId : dbData.detailsIds)
      userData.details.push_back(getInvoiceDetail(detailId));
   return userData;
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

int InvoiceDbController::getFirstInvoiceId() const
{
   return getSingleInvoiceId("ASC");
}

int InvoiceDbController::getLastInvoiceId() const
{
   return getSingleInvoiceId("DESC");
}

int InvoiceDbController::GetInvoiceId(const QDate target) const
{
   const QString queryStr = "SELECT id FROM invoice WHERE date = \"%1\"";
   auto finalStr = queryStr.arg(target.toString("dd MMM yyyy"));
   QSqlQuery query(finalStr);
   if (query.first())
      return query.value(0).toInt();
   return -1;
}

// TODO : Remove duplication between here and getInvoiceTemplateData()
InvoiceDbData InvoiceDbController::getLastInvoiceData() const
{
    InvoiceDbData data;
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
        data.detailsIds = getInvoiceElements(invoiceId);
    }
    return data;
}

InvoiceDbData InvoiceDbController::getInvoiceDbData(const int invoiceId) const
{
   InvoiceDbData data;
   data.id = invoiceId;

   const QString queryStr = "SELECT companyId, clientId, templateId, stylesheetId, date, notes, currency "
                            "FROM invoice WHERE id = %1";
   QSqlQuery query(queryStr.arg(invoiceId));
   if (query.first())
   {
      //const int companyId = query.value(0).toInt();
      const int clientId = query.value(1).toInt();
      const int templateId = query.value(2).toInt();
      const int stylesheetId = query.value(3).toInt();
      const QString dateStr = query.value(4).toString();
      const QString notes = query.value(5).toString();
      const QString currency = query.value(6).toString();

      data.date = QDate::fromString(dateStr, dateFormatStr);
      data.detailsIds = getInvoiceElements(invoiceId);
      data.notes = notes;
      data.currency = currency;
      data.clientId = clientId;
      data.templateId = templateId;
      data.stylesheetId = stylesheetId;
   }

   return data;
}

InvoiceUserData InvoiceDbController::getInvoiceUserData(const int invoiceId) const
{
   InvoiceUserData data;
   data.id = invoiceId;

   const QString queryStr = "SELECT companyId, clientId, templateId, stylesheetId, date, notes, currency "
                            "FROM invoice WHERE id = %1";
   QSqlQuery query(queryStr.arg(invoiceId));
   if (query.first())
   {
      const int startingIndex = 0;
      fillInvoiceUserData(data, query, startingIndex);
   }

   return data;
}

std::vector<InvoiceUserData> InvoiceDbController::getAllInvoiceTemplateData() const
{
   std::vector<InvoiceUserData> data;

   const QString queryStr = "SELECT id, companyId, clientId, templateId, stylesheetId, date, notes, currency "
                            "FROM invoice";
   QSqlQuery query;
   const bool result = query.exec(queryStr);
   if (result)
   {
      const int startingIndex = 1;
      while (query.next())
      {
         InvoiceUserData invoiceData;

         const int invoiceId = query.value(0).toInt();
         invoiceData.id = invoiceId;

         fillInvoiceUserData(invoiceData, query, startingIndex);

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

QString InvoiceDbController::getTemplateName(const int id) const
{
   return getFromId("name", id, templateTypeId);
}

QString InvoiceDbController::getStylesheetName(const int id) const
{
   return getFromId("name", id, stylesheetTypeId);
}

QString InvoiceDbController::getTemplateData(const int id) const
{
   return getFromId("content", id, templateTypeId);
}

QString InvoiceDbController::getStylesheetData(const int id) const
{
   return getFromId("content", id, stylesheetTypeId);
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
   bool ok = removeFromInvoiceElements(id);
   if (!ok)
      return false;

   ok = removeFromInvoiceMap(id);
   if (!ok)
      return false;

   ok = removeFromInvoiceTable(id);
   return ok;
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

int InvoiceDbController::getTotalInvoiceCount() const
{
   QSqlQuery query;
   const bool ok = query.exec("SELECT COUNT(id) FROM invoice");
   if (ok && query.next())
      return query.value(0).toInt();
   return -1;
}

int InvoiceDbController::getTotalClientCount() const
{
   QSqlQuery query;
   const bool ok = query.exec("SELECT COUNT(id) FROM company WHERE isClient = 1");
   if (ok && query.next())
      return query.value(0).toInt();
   return -1;

}

double InvoiceDbController::getTotalInvoicedAmount() const
{
   const QString queryInvoiceValue = "SELECT SUM(value * quantity) FROM invoiceelement WHERE id = %1";
   const QString queryInvoiceIds = "SELECT id FROM invoice";
   const QString queryInvoiceElts = "SELECT idElement FROM invoicedetailmap WHERE "
                                    "invoicedetailmap.idInvoice IN (%1)";

   QSqlQuery query;
   const bool ok = query.exec(queryInvoiceElts.arg(queryInvoiceIds));
   if (!ok)
      return -1.0;

   double sum = 0.0;
   while (query.next())
   {
      QSqlQuery subquery;
      const bool ok2 = subquery.exec(queryInvoiceValue.arg(query.value(0).toInt()));
      if (ok2 && subquery.next())
         sum += subquery.value(0).toDouble();
   }
   return sum;
}

int InvoiceDbController::getMonthCount() const
{
   QSqlQuery query;
   const bool ok = query.exec("SELECT date FROM invoice");
   if (!ok)
      return -1;

   const std::vector<QDate> dates = toSortedDates(query);;
   switch (dates.size())
   {
      case 0 : return 0;
      case 1 : return 1;
      default :
      {
         const QDate firstDate = dates.front();
         const QDate lastDate = dates.back();
         const int dayCount = firstDate.daysTo(lastDate);
         const int daysInMonth = 30;
         return dayCount / daysInMonth;
      };
   }
}

InvoiceDbController::IncomePerClientVec InvoiceDbController::getIncomePerClient() const
{
   IncomePerClientVec data;

   QSqlQuery clientQuery;
   bool ok = clientQuery.exec("SELECT company.name, company.id FROM company WHERE isClient = 1");
   if (!ok)
      return IncomePerClientVec();

   while (clientQuery.next())
   {
      const QString companyName = clientQuery.value(0).toString();
      const int companyId = clientQuery.value(1).toInt();
      const IncomeData clientIncomeData = getClientInvoicedValues(companyId);

      auto sumFunc = [](double total, const std::pair<QDate, double>& value)
      {
         return total + value.second;
      };
      const double clientTotal = std::accumulate(clientIncomeData.begin(), clientIncomeData.end(), 0.0, sumFunc);

      data.push_back(std::make_pair(companyName, clientTotal));
   }

   return data;
}

IncomeHistory InvoiceDbController::getIncomeHistory() const
{
   IncomeHistory history;

   QSqlQuery clientQuery;
   bool ok = clientQuery.exec("SELECT company.name, company.id FROM company WHERE isClient = 1");
   if (!ok)
      return IncomeHistory();

   const vector<QDate> datespans = createDateSpans();
   while (clientQuery.next())
   {
      const QString companyName = clientQuery.value(0).toString();
      const int companyId = clientQuery.value(1).toInt();
      const IncomeData clientIncomeData = getClientInvoicedValues(companyId);

      const std::vector<double> valuesByTimespan = toValuesByTimespan(clientIncomeData, datespans);

      history.push_back(std::make_pair(companyName, valuesByTimespan));
   }

   return history;
}

std::pair<QDate, QDate> InvoiceDbController::getBoundaryMonths() const
{
   QSqlQuery query;
   bool ok = query.exec("SELECT date FROM invoice");
   if (!ok)
      return std::pair<QDate, QDate>();

   std::vector<QDate> dates;
   while (query.next())
   {
      const QString dateStr = query.value(0).toString();
      dates.push_back(QDate::fromString(dateStr, dateFormatStr));
   }

   std::sort(dates.begin(), dates.end());
   return std::make_pair(dates.front(), dates.back());
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
      return query.value(0).toInt();
   return -1;
}

bool InvoiceDbController::createDbConnection(const QString &filename)
{
    dbFilename = filename;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    return db.open();
}

bool InvoiceDbController::createResourceTypeData(QString* errorMessage)
{
   QSqlQuery query;
   if (!query.exec("CREATE TABLE resourceType (id INTEGER primary key, name TEXT)"))
   {
      if (errorMessage)
         *errorMessage = query.lastError().text();
       return false;
   }

   query.prepare("INSERT INTO resourceType (id, name) VALUES (:id, :name)");
   query.bindValue(":id", stylesheetTypeId);
   query.bindValue(":name", "Stylesheet");
   if (!query.exec())
   {
      if (errorMessage)
         *errorMessage = query.lastError().text();
       return false;
   }

   query.bindValue(":id", templateTypeId);
   query.bindValue(":name", "Template");
   if (!query.exec())
   {
      if (errorMessage)
         *errorMessage = query.lastError().text();
       return false;
   }

   return true;
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

QString InvoiceDbController::getFromId(const QString& field, const int id, const int typeId) const
{
   QSqlQuery query;
   query.prepare(QString("SELECT %1 FROM fileresource WHERE id = :id AND typeId = :typeId").arg(field));
   query.bindValue(":id", id);
   query.bindValue(":typeId", typeId);
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
      const InvoiceDetail detail = getInvoiceDetail(elementId);
      details.push_back(detail);
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

InvoiceDetail InvoiceDbController::getInvoiceDetail(const int detailId) const
{
   const QString queryStr = "SELECT description, value, quantity FROM invoiceelement WHERE id = %1";
   QSqlQuery query;
   const bool ok = query.exec(queryStr.arg(detailId));
   if (ok && query.next())
   {
       const QString service = query.value(0).toString();
       const double value = query.value(1).toDouble();
       const double quantity = query.value(2).toDouble();
       return InvoiceDetail(service, quantity, value);
   }
   return InvoiceDetail("", 0.0);
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

int InvoiceDbController::getSingleInvoiceId(const QString& sortOrder) const
{
   const QString queryStr = "SELECT id FROM invoice ORDER BY id %1 LIMIT 1";
   QSqlQuery query;
   const bool ok = query.exec(queryStr.arg(sortOrder));
   if (ok && query.next())
       return query.value(0).toInt();
   return -1;

}

void InvoiceDbController::fillInvoiceUserData(InvoiceUserData& data, QSqlQuery& query, const int startingIndex) const
{
   const int companyId = query.value(startingIndex).toInt();
   const int clientId = query.value(startingIndex+1).toInt();
   const int templateId = query.value(startingIndex+2).toInt();
   const int stylesheetId = query.value(startingIndex+3).toInt();
   const QString dateStr = query.value(startingIndex+4).toString();
   const QString notes = query.value(startingIndex+5).toString();
   const QString currency = query.value(startingIndex+6).toString();

   data.date = QDate::fromString(dateStr, dateFormatStr);
   data.notes = notes;
   data.currency = currency;
   data.templateName = getTemplateName(templateId);
   data.templateData = getTemplateData(templateId);
   data.stylesheetName = getStylesheetName(stylesheetId);
   data.stylesheetData = getStylesheetData(stylesheetId);
   data.userCompany = getCompanyData(companyId);
   data.clientCompany = getCompanyData(clientId);
   data.details = createInvoiceDetails(data.id);
}

InvoiceDbController::IncomeData InvoiceDbController::getClientInvoicedValues(const int clientId) const
{
   IncomeData incomes;

   const QString invoiceQueryStr = "SELECT id, date FROM invoice WHERE clientId = %1";
   QSqlQuery invoiceQuery;
   bool ok = invoiceQuery.exec(invoiceQueryStr.arg(clientId));
   if (!ok)
      return IncomeData();

   while (invoiceQuery.next())
   {
      const int invoiceId = invoiceQuery.value(0).toInt();
      const QString invoiceDateStr = invoiceQuery.value(1).toString();
      const QDate invoiceDate = QDate::fromString(invoiceDateStr, dateFormatStr);

      const QString detailQueryStr = "SELECT idElement FROM invoicedetailmap WHERE idInvoice = %1";
      QSqlQuery detailQuery;
      ok = detailQuery.exec(detailQueryStr.arg(invoiceId));
      if (!ok)
         return IncomeData();

      double invoiceTotal = 0.0;
      while (detailQuery.next())
      {
         const int detailId = detailQuery.value(0).toInt();

         const QString amountQueryStr = "SELECT value * quantity FROM invoiceelement WHERE id = %1";
         QSqlQuery amountQuery;
         ok = amountQuery.exec(amountQueryStr.arg(detailId));
         if (!ok || !amountQuery.next())
            return IncomeData();

         double t = amountQuery.value(0).toDouble();;
         invoiceTotal += t;
      }

      incomes.push_back(std::make_pair(invoiceDate, invoiceTotal));
   }

   return incomes;
}

vector<QDate> InvoiceDbController::createDateSpans() const
{
   const pair<QDate, QDate> dateBoundaries = getBoundaryMonths();

   QDate firstDate(dateBoundaries.first.year(), dateBoundaries.first.month(), 1);
   QDate lastDate(dateBoundaries.second.year(), dateBoundaries.first.month(), 31);

   vector<QDate> dateSpans;
   QDate currentDate = firstDate;
   for (; currentDate < lastDate; currentDate = currentDate.addMonths(1))
      dateSpans.push_back(currentDate);
   dateSpans.push_back(currentDate);
   return dateSpans;
}

std::vector<double> InvoiceDbController::toValuesByTimespan(const IncomeData& data, const vector<QDate> dateSpans)
{
   std::vector<double> values;

    if (!data.empty())
   {
       auto itDate = dateSpans.begin();
       auto itDateEnd = dateSpans.end()-1;
       auto itIncome = data.begin();

       for (; itDate != itDateEnd; ++itDate)
       {
          double timespanTotal = 0.0;
          while (itIncome->first < *(itDate+1))
          {
             timespanTotal += itIncome->second;
             ++itIncome;
          }
          values.push_back(timespanTotal);
       }
   }
   return values;
}

std::vector<QDate> InvoiceDbController::toSortedDates(QSqlQuery& query) const
{
   std::vector<QDate> dates;
   while (query.next())
   {
      dates.push_back(QDate::fromString(query.value(0).toString(), dateFormatStr));
   }
   std::sort(dates.begin(), dates.end());
   return dates;
}
