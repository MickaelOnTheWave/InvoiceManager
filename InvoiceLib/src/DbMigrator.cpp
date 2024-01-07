#include "DbMigrator.h"

#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>

#include "InvoiceDbController.h"
#include "Utils.h"


DbMigrator::DbMigrator(InvoiceDbController& _controller)
   : controller(_controller)
{
   addToMap(9, []() {
      QSqlQuery query;
      if (!query.exec("ALTER TABLE invoiceelement ADD quantity DOUBLE DEFAULT 1.0"))
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

      return true;
   });

   addToMap(11, [this]() {

      bool ok = InvoiceDbController::createResourceTypeData();
      if (!ok)
         return false;

      ok = changeStylesheetToFileResource();
      if (!ok)
         return false;

      ok = moveTemplatesToFileResource();
      if (!ok)
         return false;

      QSqlQuery query;
      if (!query.exec("DROP TABLE template"))
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
   auto completeMigrationFunction = [migrationFunc, targetVersion]() {
      const bool ok = migrationFunc();
      if (!ok)
         return false;

      QSqlQuery query;
      query.prepare("UPDATE version SET value = :newVersion");
      query.bindValue(":newVersion", targetVersion);
      if (!query.exec())
         return false;

      return true;
   };
   const auto dataToInsert = std::make_pair(targetVersion, completeMigrationFunction);
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
      const QString stylesheetContent = Utils::readFileContent(stylesheetFile);

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

bool DbMigrator::changeStylesheetToFileResource()
{
   QSqlQuery query;
   if (!query.exec("ALTER TABLE stylesheet RENAME TO fileresource"))
         return false;

   if (!query.exec("ALTER TABLE fileresource ADD typeId INTEGER"))
         return false;

   if (!query.exec("UPDATE fileresource SET typeId = 1"))
         return false;

   return true;
}

bool DbMigrator::moveTemplatesToFileResource()
{
   QSqlQuery query;
   if (!query.exec("SELECT id, name, content FROM template"))
   {
      return false;
   }

   while (query.next())
   {
      const int oldTemplateId = query.value(0).toInt();
      const QString name = query.value(1).toString();
      const QString content = query.value(2).toString();

      QSqlQuery insertQuery;
      insertQuery.prepare("INSERT INTO fileresource (name, content, typeId) VALUES (:name, :content, :type)");
      insertQuery.bindValue(":name", name);
      insertQuery.bindValue(":content", content);
      insertQuery.bindValue(":type", 2);
      if (!insertQuery.exec())
         return false;

      const int newTemplateId = insertQuery.lastInsertId().toInt();
      QSqlQuery updateQuery;
      updateQuery.prepare("UPDATE invoice SET templateId = :templateId WHERE templateId = :oldId");
      updateQuery.bindValue(":templateId", newTemplateId);
      updateQuery.bindValue(":oldId", oldTemplateId);
      if (!updateQuery.exec())
         return false;
   }

   return true;
}
