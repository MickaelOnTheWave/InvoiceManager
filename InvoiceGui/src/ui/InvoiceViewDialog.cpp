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

#include "InvoiceViewDialog.h"
#include "ui_InvoiceViewDialog.h"

#include <QFile>
#include <QMessageBox>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>

InvoiceViewDialog::InvoiceViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvoiceViewDialog)
{
    ui->setupUi(this);
    connect(ui->deleteButton, &QAbstractButton::clicked, this, &InvoiceViewDialog::onDeleteClicked);
    connect(ui->closeButton, &QAbstractButton::clicked, this, &QDialog::close);
    connect(ui->createPdfButton, &QAbstractButton::clicked, this, &InvoiceViewDialog::onCreatePdf);
}

InvoiceViewDialog::~InvoiceViewDialog()
{
   delete ui;
}

void InvoiceViewDialog::setData(const InvoiceUserData& data)
{
   invoiceData = data;
   const QString templateContent = readFileContent(data.templatePath);
   const QString previewContent = fillTemplate(templateContent, data);

   ui->viewWidget->show(previewContent, data.stylesheetPath);
}

void InvoiceViewDialog::onDeleteClicked()
{
   const QString message = tr("This invoice will be permanently removed from the database."
                              "Are you sure you want to proceed ?");
   const auto selectedButton = QMessageBox::question(nullptr, "Removal", message);
   if (selectedButton == QMessageBox::Yes)
   {
      emit deleteConfirmed(invoiceData.id);
      close();
   }
}

void InvoiceViewDialog::onCreatePdf()
{
   const QString templateContent = readFileContent(invoiceData.templatePath);
   const QString previewContent = fillTemplate(templateContent, invoiceData);


   QTextDocument *document = new QTextDocument();
   document->setHtml(previewContent);
   document->setDefaultStyleSheet(invoiceData.stylesheetPath);

   QPrinter printer(QPrinter::HighResolution);
   printer.setPageSize(QPrinter::A4);
   printer.setOutputFormat(QPrinter::PdfFormat);

   printer.setOutputFileName("/home/mickael/2023-09 Consulting Invoice.pdf");

   document->print(&printer);
}

QString InvoiceViewDialog::readFileContent(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return QString();
    QTextStream in(&f);
    return in.readAll();
}

QString InvoiceViewDialog::fillTemplate(const QString& templateModel,
                                        const InvoiceUserData& data)
{
   QString filledTemplate = templateModel;

   filledTemplate.replace("{ID}", QString::number(data.id));

   const QString dateStr = QLocale(QLocale::English).toString(data.date, "d MMM yyyy");
   filledTemplate.replace("{DATE}", dateStr);

   filledTemplate.replace("{USER-COMPANY-NAME}", data.userCompany.name);
   filledTemplate.replace("<p>{USER-COMPANY-ADDRESS}</p>", buildReplaceAddress(data.userCompany.address));
   filledTemplate.replace("{USER-COMPANY-EMAIL}", data.userCompany.email);

   filledTemplate.replace("{CLIENT-NAME}", data.clientCompany.name);
   filledTemplate.replace("<p>{CLIENT-ADDRESS}</p>", buildReplaceAddress(data.clientCompany.address));

   filledTemplate.replace("<tr>{INVOICE-DETAILS}</tr>", buildReplaceDetails(data.details));
   filledTemplate.replace("{INVOICE-TOTAL}", buildInvoiceTotal(data.details));

   filledTemplate.replace("{CURRENCY}", data.currency);
   filledTemplate.replace("{NOTES}", data.notes);

   {
      // Debug code only
      QFile f("/home/mickael/Prog/InvoiceManage/builds/Debug/outputTemplate.html");
      f.open(QFile::ReadWrite | QFile::Text);
      f.write(filledTemplate.toUtf8());
   }

   return filledTemplate;
}

QString InvoiceViewDialog::buildReplaceAddress(const QString& recordedAddress)
{
   QString replacedAddress;
   const QStringList explodedAddress = recordedAddress.split("\n");
   for (int i=0; i < explodedAddress.size(); ++i)
   {
       const QString currentPart = explodedAddress[i];
       if (!currentPart.isEmpty())
           replacedAddress += QString("<p>%1</p>").arg(currentPart);
   }
   return replacedAddress;
}

QString InvoiceViewDialog::buildReplaceDetails(const std::vector<InvoiceDetail>& details)
{
   QString replacedStr;
   for (const auto& detailLine : details)
   {
       QString valueStr = QString::asprintf("%.2f", detailLine.value);

       const QString nameCell = QString("\t\t\t<td>%1</td>").arg(detailLine.description);
       const QString valueCell = QString("\t\t\t<td>%1</td>").arg(valueStr);
       replacedStr += QString("<tr>\n%1\n%2\n\t\t</tr>\n").arg(nameCell, valueCell);
   }
   return replacedStr;
}

QString InvoiceViewDialog::buildInvoiceTotal(const std::vector<InvoiceDetail>& details)
{
   auto accumulator = [](const double partialSum, const InvoiceDetail& detail)
   {
      return partialSum + detail.value;
   };
   const double totalValue = std::accumulate(details.begin(), details.end(), 0.0, accumulator);
   return QString::asprintf("%.2f", totalValue);
}
