#ifndef IDPARENTINGMAP_H
#define IDPARENTINGMAP_H

#include <initializer_list>
#include <map>
#include <vector>

class IdParentingMap
{
public:
   using DataMap = std::map<int, std::vector<int>>;
   using DataMapIt = DataMap::iterator;
   using DataMapConstIt = DataMap::const_iterator;

   IdParentingMap();
   IdParentingMap(std::initializer_list<DataMap::value_type> initList);

   DataMapConstIt findParentIt(const int id) const;

   /** Forwards from std::map  **/

   DataMap::iterator find(const DataMap::key_type& k);
   DataMap::mapped_type& operator[](const std::size_t i);
   std::size_t size() const;
   DataMapIt end();
   DataMapConstIt end() const;
   DataMap::iterator erase(const DataMap::iterator it);

private:
   DataMap data;
};

#endif // IDPARENTINGMAP_H
