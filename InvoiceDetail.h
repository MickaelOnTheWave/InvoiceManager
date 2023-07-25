#ifndef INVOICEDETAIL_H
#define INVOICEDETAIL_H

#include <QString>

class InvoiceDetail
{
public:
    InvoiceDetail(const QString& _description = "",
                  const double _value = 0.0);

    QString description;
    double value;
};

#endif // INVOICEDETAIL_H
