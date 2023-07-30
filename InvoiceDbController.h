#ifndef INVOICEDBCONTROLLER_H
#define INVOICEDBCONTROLLER_H

#include <QtSql/qsqldatabase.h>
#include <QStringListModel>
#include <QObject>
#include <QString>

#include "Company.h"
#include "InvoiceDetail.h"

class InvoiceDbController : public QObject
{
    Q_OBJECT

public:
    static const int currentDbVersion = 5;

    InvoiceDbController();

    bool createDb(const QString& filename);
    bool openDb(const QString& filename);
    void closeDb();

    QString getLastError() const;

    bool writeUserCompany(const CompanyData& company);
    std::vector<int> writeInvoiceDetails(const std::vector<InvoiceDetail>& details);

    bool writeInvoice(const int invoiceId, const int clientId, const int stylesheetId,
                      const std::vector<int>& detailsIds, const QDate& date);

    QString getUserCompanyName() const;
    int getUserCompanyId() const;

    int getLastUsedInvoiceId() const;

    QString getDatabaseFile() const;
    int getDatabaseVersion() const;

    QSqlDatabase getDatabase();

    static QSqlQuery createWriteCompanyQuery(const CompanyData& data, const bool isClient);

private:
    bool createDbConnection(const QString& filename);

    static QString createUserCompanyRequest(const QString& field);
    QString createInvoiceDetailsWriteQuery(const std::vector<InvoiceDetail>& details) const;

    int writeToInvoiceTable(const int invoiceId, const int clientId, const int stylesheetId, const QDate& date);
    bool writeToInvoiceMapTable(const int invoiceId, const std::vector<int>& detailsIds);

    QString dbFilename;
    QSqlDatabase db; // TODO : remove this. See tip from Qt documentqtion.
    QString lastErrorMessage;
};

#endif // INVOICEDBCONTROLLER_H
