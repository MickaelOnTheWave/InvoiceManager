#include "CompanyData.h"

CompanyData::CompanyData()
{

}

CompanyData::CompanyData(const QString& _name, const QString& _address, const QString& _email)
    : name(_name), address(_address), email(_email)
{
}
