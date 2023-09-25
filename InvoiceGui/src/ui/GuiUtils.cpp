/*
 * InvoiceManager
 * Copyright (C) 2023 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
