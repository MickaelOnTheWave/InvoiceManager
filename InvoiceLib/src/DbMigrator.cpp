#include "DbMigrator.h"

#include <QSqlQuery>


DbMigrator::DbMigrator(InvoiceDbController& _controller)
   : controller(_controller)
{
   addToMap(9, []() {
      QSqlQuery query;
      if (!query.exec("ALTER TABLE invoiceelement ADD quantity DOUBLE DEFAULT 1.0"))
            return false;
      if (!query.exec("UPDATE version SET value = 9"))
            return false;
      return true;
   });
}

bool DbMigrator::Migrate()
{
   const int dbVersion = controller.getDatabaseVersion();
   if (dbVersion < 8)
   {
      // Versioning the DB started at version 3, and
      // Version migrations started at version 9.
      return false;
   }

   auto itMigrator = migrationFunctions.find(dbVersion+1);
   auto endMigrator = migrationFunctions.end();
   for (; itMigrator != endMigrator; ++itMigrator)
   {
      const bool ok = itMigrator->second();
      if (!ok)
         return false;
   }
   return true;
}

void DbMigrator::addToMap(const int targetVersion, std::function<bool ()> migrationFunc)
{
   const auto dataToInsert = std::make_pair(targetVersion, migrationFunc);
   migrationFunctions.insert(dataToInsert);
}
