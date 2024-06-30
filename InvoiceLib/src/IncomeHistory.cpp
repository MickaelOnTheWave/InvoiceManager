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

void IncomeHistory::push_back(const DataValue& newData)
{
   data.push_back(newData);
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

