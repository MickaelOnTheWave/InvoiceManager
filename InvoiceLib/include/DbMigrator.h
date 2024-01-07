#ifndef DBMIGRATOR_H
#define DBMIGRATOR_H

#include "InvoiceDbController.h"

class DbMigrator
{
public:
   DbMigrator(InvoiceDbController& _controller);

   bool Migrate();

private:
   void addToMap(const int targetVersion, std::function<bool()> migrationFunc);
   bool changeTableToContents(const QString tableName);
   bool changeStylesheetToFileResource();
   bool moveTemplatesToFileResource();

   std::map<int, std::function<bool()>> migrationFunctions;
   InvoiceDbController& controller;
};

#endif
