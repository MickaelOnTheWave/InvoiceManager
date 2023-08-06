#ifndef GUIUTILS_H
#define GUIUTILS_H

#include "FileResourceModel.h"

class GuiUtils
{
public:
    static void OnAddTemplate(FileResourceModel* model);
    static void OnAddStylesheet(FileResourceModel* model);
    static void addDataToModel(QWidget *dataWidget, const QString &dialogTitle, std::function<bool ()> insertDataFunc);

private:
    static void OnAddFileResource(FileResourceModel* model, const QString &fileFilter, const QString &dialogTitle);

};

#endif // GUIUTILS_H
