#ifndef COMPANY_H
#define COMPANY_H

#include <QString>

class CompanyData
{
public:
    CompanyData();
    CompanyData(const QString& _name, const QString& _address, const QString& _email);

    QString name;
    QString address;
    QString email;
    QString phoneNumber;
};

#endif // COMPANY_H
