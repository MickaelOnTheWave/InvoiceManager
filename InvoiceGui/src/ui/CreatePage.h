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

#ifndef CreatePage_H
#define CreatePage_H

#include <QWidget>

#include "CompanyData.h"
#include "src/FileResourceModel.h"

namespace Ui {
class CreatePage;
}

class CreatePage : public QWidget
{
    Q_OBJECT

public:
    explicit CreatePage(QWidget *parent = nullptr);
    ~CreatePage();

    void reset();

    CompanyData getCompanyData() const;

    void setModel(FileResourceModel* _templateModel,
                  FileResourceModel* _stylesheetModel);

signals:
    void confirm();
    void cancel();

private slots:
    void onAddStylesheet();
    void onAddTemplate();

private:
    Ui::CreatePage *ui;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
};

#endif // CreatePage_H
