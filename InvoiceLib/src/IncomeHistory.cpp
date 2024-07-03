#include "IncomeHistory.h"

double IncomeHistory::getMaxValue() const
{
   std::vector<double> totalsByDateSpan;
   for (int i=0; i<data.front().second.size(); ++i)
      totalsByDateSpan.push_back(0.0);

   for(const auto& clientIncome : data)
   {
      auto itTotals = totalsByDateSpan.begin();
      for (const auto& incomeData : clientIncome.second)
      {
         *itTotals += incomeData;
         ++itTotals;
      }
   }

   auto itMaxElement = max_element(totalsByDateSpan.begin(), totalsByDateSpan.end());
   return (itMaxElement != totalsByDateSpan.end()) ? *itMaxElement : 0.0;
}

IncomeHistory::DataMap::iterator IncomeHistory::find(const QString &key)
{
   for (auto it=data.begin(); it!=data.end(); ++it)
   {
      if (it->first == key)
         return it;
   }
   return data.end();
}

void IncomeHistory::push_back(const DataValue& newData)
{
   data.push_back(newData);
}

const size_t IncomeHistory::size() const
{
   return data.size();
}

IncomeHistory::DataMap::iterator IncomeHistory::begin()
{
   return data.begin();
}

IncomeHistory::DataMap::iterator IncomeHistory::end()
{
   return data.end();
}

IncomeHistory::DataMap::const_iterator IncomeHistory::begin() const
{
   return data.begin();
}

IncomeHistory::DataMap::const_iterator IncomeHistory::end() const
{
   return data.end();
}

