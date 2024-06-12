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

#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>

#include <QAbstractItemModel>

#include "ClientModel.h"
#include "FileResourceModel.h"
#include "InvoiceDbController.h"

namespace Ui {
class MorePage;
}

class MorePage : public QWidget
{
    Q_OBJECT

public:
    explicit MorePage(QWidget *parent = nullptr);
    ~MorePage();

   void setCompanyTitle(const QString& title);

   void setController(InvoiceDbController* _controller);

    void connectViewsToModels(ClientModel* _clientModel, FileResourceModel *_templateModel,
                              FileResourceModel* _stylesheetModel);

signals:
    void back();
    void settingsClicked();
    void aboutClicked();

private slots:
    void onAddClient();
    void onAddTemplate();
    void onAddStylesheet();
    void onRemoveClient(const QModelIndex index);
    void onRemoveTemplate(const QModelIndex index);
    void onRemoveStylesheet(const QModelIndex index);

private:
    void addDataToModel(QWidget* dataWidget, std::function<bool()> insertDataFunc);
    bool canRemoveFileResource(const int id, const QString& dbField) const;
    void onRemoveFileResource(const QModelIndex index, FileResourceModel* model,
                              const QString& dbField);
    void onRemoveFromModel(const QModelIndex& index, QAbstractItemModel* model,
                           const QString& dbField,std::function<void(const QModelIndex&)> deleteFunc);

    Ui::MorePage *ui;
    ClientModel* clientModel;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
    InvoiceDbController* controller = nullptr;
};

#endif // MOREPAGE_H
