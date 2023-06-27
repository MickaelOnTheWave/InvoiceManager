#include "Company.h"

Company::Company()
{

}

Company::Company(const QString& _name, const QString& _address, const QString& _email)
    : name(_name), address(_address), email(_email)
{
}
