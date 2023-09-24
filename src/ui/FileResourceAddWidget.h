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

#ifndef FileResourceAddWidget_H
#define FileResourceAddWidget_H

#include <QWidget>

namespace Ui {
class FileResourceAddWidget;
}

class FileResourceAddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileResourceAddWidget(const QString& fileFilter,
                                   QWidget *parent = nullptr);
    ~FileResourceAddWidget();

    QString getName() const;
    QString getPath() const;

private slots:
    void onBrowseClick();

private:
    Ui::FileResourceAddWidget *ui;
    QString typeDescription;
};

#endif // FileResourceAddWidget_H
