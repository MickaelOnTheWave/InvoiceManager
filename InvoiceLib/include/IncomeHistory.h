#ifndef INCOMEHISTORY_H
#define INCOMEHISTORY_H

#include <QString>

class IncomeHistory
{
public:
   using DataValue = std::pair<QString, std::vector<double>>;
   using DataMap = std::vector<DataValue>;

   double getMaxValue() const;

   DataMap::iterator find(const QString& key);


   /** Forwards from std::vector  **/
   void push_back(const DataValue& newData);
   const size_t size() const;
   DataMap::iterator begin();
   DataMap::iterator end();
   DataMap::const_iterator begin() const;
   DataMap::const_iterator end() const;


private:
   DataMap data;
};

#endif // INCOMEHISTORY_H
