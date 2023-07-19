#include "GuiUtils.h"

#include <QMessageBox>
#include "NewDataDialog.h"
#include "StylesheetDetailsWidget.h"

void GuiUtils::OnAddStylesheet(StylesheetModel *model)
{
    auto contentWidget = new StylesheetDetailsWidget();
    addDataToModel(contentWidget, [model, contentWidget] () {
        return model->insertAtEnd(contentWidget->getName(),
                                  contentWidget->getPath());
    });
}

void GuiUtils::addDataToModel(QWidget *dataWidget, std::function<bool ()> insertDataFunc)
{
    NewDataDialog dialog(dataWidget);
    const int result = dialog.exec();
    if (result == QDialog::Accepted)
    {
        const bool ok = insertDataFunc();
        if (!ok)
            QMessageBox::warning(&dialog, "Error", "Could not insert data into model");
    }
}
