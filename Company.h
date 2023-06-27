#ifndef COMPANY_H
#define COMPANY_H

#include <QString>

class Company
{
public:
    Company();
    Company(const QString& _name, const QString& _address, const QString& _email);

    QString name;
    QString address;
    QString email;
};

#endif // COMPANY_H
