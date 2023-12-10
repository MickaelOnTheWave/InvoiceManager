#include "DbMigrator.h"

#include <QFile>
#include <QSqlQuery>
#include <QTextStream>


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

   addToMap(10, [this]() {
      bool ok = changeTableToContents("stylesheet");
      if (!ok)
         return false;

      ok = changeTableToContents("template");
      if (!ok)
         return false;

      QSqlQuery query;
      if (!query.exec("UPDATE version SET value = 10"))
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

bool DbMigrator::changeTableToContents(const QString tableName)
{
   QSqlQuery query;
   QString queryStr = "ALTER TABLE %1 ADD content TEXT";
   if (!query.exec(queryStr.arg(tableName)))
         return false;

   queryStr = "SELECT id, file FROM %1";
   if (!query.exec(queryStr.arg(tableName)))
         return false;
   while (query.next())
   {
      const QString stylesheetId = query.value(0).toString();
      const QString stylesheetFile = query.value(1).toString();
      const QString stylesheetContent = readFileContent(stylesheetFile);

      QSqlQuery updateQuery;
      queryStr = "UPDATE %1 SET content = :content WHERE id = :idStylesheet";
      updateQuery.prepare(queryStr.arg(tableName));
      updateQuery.bindValue(":content", stylesheetContent);
      updateQuery.bindValue(":idStylesheet", stylesheetId);
      if (!updateQuery.exec())
         return false;
   }

   queryStr = "ALTER TABLE %1 DROP COLUMN file";
   if (!query.exec(queryStr.arg(tableName)))
         return false;

   return true;
}

QString DbMigrator::readFileContent(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return QString();
    QTextStream in(&f);
    return in.readAll();
}
