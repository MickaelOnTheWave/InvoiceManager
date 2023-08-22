#include "GuiUtils.h"

#include <QMessageBox>
#include "NewDataDialog.h"
#include "FileResourceAddWidget.h"

void GuiUtils::OnAddTemplate(FileResourceModel *model)
{
    OnAddFileResource(model, "Template (*.html)", "Select a template");
}

void GuiUtils::OnAddStylesheet(FileResourceModel *model)
{
    OnAddFileResource(model, "Stylesheet (*.css)", "Select a Stylesheet");
}

void GuiUtils::addDataToModel(QWidget *dataWidget, const QString& dialogTitle,
                              std::function<bool ()> insertDataFunc)
{
    NewDataDialog dialog(dataWidget);
    dialog.setWindowTitle(dialogTitle);
    const int result = dialog.exec();
    if (result == QDialog::Accepted)
    {
        const bool ok = insertDataFunc();
        if (!ok)
            QMessageBox::warning(&dialog, "Error", "Could not insert data into model");
    }
}

void GuiUtils::OnAddFileResource(FileResourceModel *model, const QString& fileFilter, const QString& dialogTitle)
{
    auto contentWidget = new FileResourceAddWidget(fileFilter);
    addDataToModel(contentWidget, dialogTitle, [model, contentWidget] () {
        return model->insertAtEnd(contentWidget->getName(),
                                  contentWidget->getPath());
    });
}
