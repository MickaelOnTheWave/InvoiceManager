#ifndef COMPANYCHILDMAP_H
#define COMPANYCHILDMAP_H

#include <map>

class CompanyChildMap
{
   public:
   using DataMap = std::map<int, int>;

   DataMap::const_iterator findParentIt(const int id) const;

   /** Forwards from std::map  **/
   DataMap::mapped_type& operator[](const std::size_t i);
   DataMap::const_iterator begin() const;
   DataMap::const_iterator end() const;

   private:
   DataMap data;
};

#endif // COMPANYCHILDMAP_H
