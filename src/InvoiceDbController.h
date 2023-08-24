#ifndef INVOICEDBCONTROLLER_H
#define INVOICEDBCONTROLLER_H

#include <QtSql/qsqldatabase.h>
#include <QStringListModel>
#include <QObject>
#include <QString>

#include "CompanyData.h"
#include "InvoiceData.h"
#include "InvoiceDetail.h"

class InvoiceDbController : public QObject
{
    Q_OBJECT

public:
    static const int currentDbVersion = 7;

    InvoiceDbController();

    bool createDb(const QString& filename);
    bool openDb(const QString& filename);
    void closeDb();

    QString getLastError() const;

    bool writeUserCompany(const CompanyData& company);
    std::vector<int> writeInvoiceDetails(const std::vector<InvoiceDetail>& details);

    bool writeInvoice(const int invoiceId, const int clientId, const int templateId, const int stylesheetId,
                      const std::vector<int>& detailsIds, const QDate& date, const QString &notes, const QString &currency);

    bool invoiceExists(const int id) const;

    QString getUserCompanyName() const;
    int getUserCompanyId() const;
    CompanyData getUserCompanyData() const;

    int getLastUsedInvoiceId() const;

    InvoiceData getLastInvoiceData() const;
    InvoiceTemplateData getInvoiceTemplateData(const int invoiceId) const;

    QString getDatabaseFile() const;
    int getDatabaseVersion() const;

    QString getTemplateFilename(const int id) const;
    QString getStylesheetFilename(const int id) const;

    int getInvoiceCountUsingFile(const int id, const QString& fieldName) const;

    QSqlDatabase getDatabase();

    static QSqlQuery createWriteCompanyQuery(const CompanyData& data, const bool isClient);

private:
    bool createDbConnection(const QString& filename);

    static QString createUserCompanyRequest(const QString& field);
    QString createInvoiceDetailsWriteQuery(const std::vector<InvoiceDetail>& details) const;

    int writeToInvoiceTable(const int invoiceId, const int clientId, const int templateId, const int stylesheetId,
                            const QDate& date, const QString &notes, const QString &currency);
    bool writeToInvoiceMapTable(const int invoiceId, const std::vector<int>& detailsIds);

    QString getFilenameFromId(const QString& table, const int id) const;

    CompanyData getCompanyData(const int id) const;

    std::vector<InvoiceDetail> createInvoiceDetails(const int id) const;

    QString dbFilename;
    QSqlDatabase db; // TODO : remove this. See tip from Qt documentation.
    QString lastErrorMessage;
    const QString dateFormatStr = "d MMM yyyy";
};

#endif // INVOICEDBCONTROLLER_H
