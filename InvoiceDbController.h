#ifndef INVOICEDBCONTROLLER_H
#define INVOICEDBCONTROLLER_H

#include <QtSql/qsqldatabase.h>
#include <QStringListModel>
#include <QObject>
#include <QString>

class InvoiceDbController : public QObject
{
    Q_OBJECT

public:
    InvoiceDbController();

    void createDb(const QString& filename);
    void openDb(const QString& filename);
    void closeDb();

    void write(const QString& companyName, QStringListModel* stylesheetsModel);

    QString getCompanyName() const;

private:
    QString dbFilename;
    QSqlDatabase db;
};

#endif // INVOICEDBCONTROLLER_H
