#ifndef GUIUTILS_H
#define GUIUTILS_H

#include "StylesheetModel.h"

class GuiUtils
{
public:

    static void OnAddStylesheet(StylesheetModel* model);
    static void addDataToModel(QWidget *dataWidget, std::function<bool ()> insertDataFunc);
};

#endif // GUIUTILS_H
