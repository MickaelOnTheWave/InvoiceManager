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

#ifndef INVOICEDBCONTROLLER_H
#define INVOICEDBCONTROLLER_H

#include <QtSql/qsqldatabase.h>
#include <QStringListModel>
#include <QObject>
#include <QString>

#include "CompanyData.h"
#include "InvoiceData.h"
#include "InvoiceDetail.h"

class IncomeHistory
{
public:
   using ClientIncomeHistory = std::pair<QString, std::vector<double>>;

   double getMaxValue() const;

   void push_back(const ClientIncomeHistory& newData);

   std::vector<ClientIncomeHistory>::iterator begin();
   std::vector<ClientIncomeHistory>::iterator end();
   std::vector<ClientIncomeHistory>::const_iterator begin() const;
   std::vector<ClientIncomeHistory>::const_iterator end() const;


private:

   std::vector<ClientIncomeHistory> data;
};

class InvoiceDbController : public QObject
{
    Q_OBJECT

public:
    static const int currentDbVersion = 11;

    static const int stylesheetTypeId = 1;
    static const int templateTypeId = 2;

    using IncomePerClientVec = std::vector<std::pair<QString, double>>;
    using IncomeData = std::vector<std::pair<QDate, double>>;

    InvoiceDbController();

    bool createDb(const QString& filename);
    bool openDb(const QString& filename);
    void closeDb();

    static bool createResourceTypeData(QString* errorMessage = nullptr);
    QString getLastError() const;

    bool writeUserCompany(const CompanyData& company);
    bool writeUpdatedCompany(const CompanyData& company, const int parentCompanyId);
    std::vector<int> writeInvoiceDetails(const std::vector<InvoiceDetail>& details);

    bool writeInvoice(const int invoiceId, const int clientId, const int templateId, const int stylesheetId,
                      const std::vector<int>& detailsIds, const QDate& date, const QString &notes, const QString &currency);

    bool writeInvoice(const InvoiceDbData& data);

    bool invoiceExists(const int id) const;

    InvoiceUserData toUserData(const InvoiceDbData& dbData) const;

    QString getUserCompanyName() const;
    int getUserCompanyId() const;
    CompanyData getUserCompanyData() const;

    InvoiceDetail getInvoiceDetail(const int detailId) const;

    int getFirstInvoiceId() const;
    int getLastInvoiceId() const;
    int GetInvoiceId(const QDate target) const;

    InvoiceDbData getLastInvoiceData() const;
    InvoiceDbData getInvoiceDbData(const int invoiceId) const;
    InvoiceUserData getInvoiceUserData(const int invoiceId) const;
    std::vector<InvoiceUserData> getAllInvoiceTemplateData() const;

    QString getDatabaseFile() const;
    int getDatabaseVersion() const;

    QString getTemplateName(const int id) const;
    QString getStylesheetName(const int id) const;
    QString getTemplateData(const int id) const;
    QString getStylesheetData(const int id) const;

    int getInvoiceCountUsingFile(const int id, const QString& fieldName) const;

    bool removeInvoice(const int id);

    int getParentCompanyId(const int id);
    bool updateCompanyParenting(const int targetId, const int childId);

    static QSqlQuery createWriteCompanyQuery(const CompanyData& data, const bool isClient);

    /** For Charts Page - Used for statistics **/
    int getTotalInvoiceCount() const;
    int getTotalClientCount() const;
    double getTotalInvoicedAmount() const;
    int getMonthCount() const;

    IncomePerClientVec getIncomePerClient() const;
    IncomeHistory getIncomeHistory() const;
    std::pair<QDate, QDate> getBoundaryMonths() const;


private:
    bool createDbConnection(const QString& filename);

    static QString createUserCompanyRequest(const QString& field);
    QString createInvoiceDetailsWriteQuery(const std::vector<InvoiceDetail>& details) const;

    int writeToInvoiceTable(const int invoiceId, const int clientId, const int templateId, const int stylesheetId,
                            const QDate& date, const QString &notes, const QString &currency);
    bool writeToInvoiceMapTable(const int invoiceId, const std::vector<int>& detailsIds);

    QString getFromId(const QString& field, const int id, const int typeId) const;

    CompanyData getCompanyData(const int id) const;

    std::vector<InvoiceDetail> createInvoiceDetails(const int id) const;
    std::vector<int> getInvoiceElements(const int invoiceId) const;

    bool removeFromInvoiceTable(const int id);
    bool removeFromInvoiceElements(const int id);
    bool removeFromInvoiceMap(const int id);

    int getSingleInvoiceId(const QString& sortOrder) const;

    void fillInvoiceUserData(InvoiceUserData& data, QSqlQuery& query, const int startingIndex) const;

    IncomeData getClientInvoicedValues(const int clientId) const;

    std::vector<QDate> createDateSpans() const;

    static std::vector<double> toValuesByTimespan(const IncomeData& data, const std::vector<QDate> dateSpans);

    std::vector<QDate> toSortedDates(QSqlQuery& query) const;

    QString dbFilename;
    QString lastErrorMessage;
    const QString dateFormatStr = "d/MM/yyyy";
};

#endif // INVOICEDBCONTROLLER_H
