#ifndef INVOICEDBCONTROLLER_H
#define INVOICEDBCONTROLLER_H

#include <QObject>
#include <QString>

class InvoiceDbController : public QObject
{
    Q_OBJECT

public:
    InvoiceDbController();

    void createDb(const QString& filename);
    void openDb(const QString& filename);
    void save();
    void closeDb();

private:
    QString dbFilename;
};

#endif // INVOICEDBCONTROLLER_H
