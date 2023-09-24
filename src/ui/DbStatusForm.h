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

#ifndef DBSTATUSFORM_H
#define DBSTATUSFORM_H

#include <QWidget>

namespace Ui {
class DbStatusForm;
}

class DbStatusForm : public QWidget
{
    Q_OBJECT

public:
    explicit DbStatusForm(QWidget *parent = nullptr);
    ~DbStatusForm();

    void setData(const QString& dbFile, const int dbVersion);

private:
    Ui::DbStatusForm *ui;
};

#endif // DBSTATUSFORM_H
