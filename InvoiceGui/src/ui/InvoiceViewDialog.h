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

#ifndef INVOICEVIEWDIALOG_H
#define INVOICEVIEWDIALOG_H

#include <QDialog>

#include "InvoiceData.h"

namespace Ui {
class InvoiceViewDialog;
}

class InvoiceViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InvoiceViewDialog(QWidget *parent = nullptr);
    virtual ~InvoiceViewDialog();

    void setData(const InvoiceUserData& data);

signals:
    void deleteConfirmed(const int id);

private slots:
    void onDeleteClicked();
    void onCreatePdf();

private:
    // TODO refactor this properly
    static QString readFileContent(const QString& filename);
    static QString fillTemplate(const QString& templateModel,
                                const InvoiceUserData& data);

    static QString buildReplaceAddress(const QString& recordedAddress);
    static QString buildReplaceDetails(const std::vector<InvoiceDetail>& details);
    static QString buildInvoiceTotal(const std::vector<InvoiceDetail>& details);

    Ui::InvoiceViewDialog *ui;
    InvoiceUserData invoiceData;
};

#endif // INVOICEVIEWDIALOG_H
