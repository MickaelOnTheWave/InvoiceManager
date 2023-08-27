#include "CompanyData.h"

CompanyData::CompanyData(const QString& _name, const QString& _address, const QString& _email)
    : name(_name), address(_address), email(_email)
{
}

bool CompanyData::operator==(const CompanyData& other) const
{
   return name == other.name && address == other.address &&
         email == other.email && phoneNumber == other.phoneNumber;
}
