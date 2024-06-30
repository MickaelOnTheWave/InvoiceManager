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
