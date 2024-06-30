#include "IdParentingMap.h"

#include <algorithm>

using namespace std;

IdParentingMap::IdParentingMap()
{
}

IdParentingMap::IdParentingMap(initializer_list<DataMap::value_type> initList)
  : data(initList)
{
}

IdParentingMap::DataMapConstIt IdParentingMap::findParentIt(const int id) const
{
   for (auto it = data.begin(); it != data.end(); ++it)
   {
      auto itChild = std::find(it->second.begin(), it->second.end(), id);
      if (itChild != it->second.end())
         return it;
   }
   return data.end();
}

IdParentingMap::DataMap::iterator IdParentingMap::find(const DataMap::key_type &k)
{
   return data.find(k);
}

IdParentingMap::DataMap::mapped_type &IdParentingMap::operator[](const std::size_t i)
{
   return data[i];
}

std::size_t IdParentingMap::size() const
{
   return data.size();
}

IdParentingMap::DataMapIt IdParentingMap::end()
{
   return data.end();
}

IdParentingMap::DataMapConstIt IdParentingMap::end() const
{
   return data.end();
}

IdParentingMap::DataMap::iterator IdParentingMap::erase(const DataMap::iterator it)
{
   return data.erase(it);
}
