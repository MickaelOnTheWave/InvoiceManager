#include "CompanyChildMap.h"

CompanyChildMap::DataMap::const_iterator CompanyChildMap::findParentIt(const int id) const
{
   for (auto it = data.begin(); it != data.end(); ++it)
   {
      if (it->second == id)
         return it;
   }
   return data.end();
}

CompanyChildMap::DataMap::mapped_type &CompanyChildMap::operator[](const std::size_t i)
{
   return data[i];
}

CompanyChildMap::DataMap::const_iterator CompanyChildMap::begin() const
{
   return data.begin();
}

CompanyChildMap::DataMap::const_iterator CompanyChildMap::end() const
{
   return data.end();
}
