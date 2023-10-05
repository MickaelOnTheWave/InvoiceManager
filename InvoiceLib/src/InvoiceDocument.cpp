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

#include "InvoiceDocument.h"

#include <QFile>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>

void InvoiceDocument::setData(const InvoiceUserData& data)
{
   invoiceData = data;
}

QString InvoiceDocument::CreateHtmlContent() const
{
   const QString templateContent = readFileContent(invoiceData.templatePath);
   return fillTemplate(templateContent, invoiceData);
}

QString InvoiceDocument::CreateStyledHtmlContent() const
{
   const QString htmlContent = CreateHtmlContent();
   const QString cssContent = readFileContent(invoiceData.stylesheetPath);
   return addInternalCss(htmlContent, cssContent);
}

void InvoiceDocument::CreatePdfFile(const QString& file)
{
   QTextDocument *document = new QTextDocument();
   document->setHtml(CreateStyledHtmlContent());

   QPrinter printer(QPrinter::HighResolution);
   //printer.pageLayout().setPageSize(QPageSize::A4);
   printer.setOutputFormat(QPrinter::PdfFormat);

   printer.setOutputFileName(file);

   document->print(&printer);
}

void InvoiceDocument::CreatePdfFileFromPattern(const QString& filenamePattern)
{
   return CreatePdfFile(GetFileFromPattern(filenamePattern));
}

int InvoiceDocument::GetInvoiceId() const
{
   return invoiceData.id;
}

QString InvoiceDocument::readFileContent(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return QString();
    QTextStream in(&f);
    return in.readAll();
}

QString InvoiceDocument::fillTemplate(const QString& templateModel,
                                        const InvoiceUserData& data)
{
   QString filledTemplate = templateModel;

   filledTemplate.replace("{ID}", QString::number(data.id));

   const QString dateStr = QLocale(QLocale::English).toString(data.date, "d MMM yyyy");
   filledTemplate.replace("{DATE}", dateStr);

   filledTemplate.replace("{USER-COMPANY-NAME}", data.userCompany.name);
   filledTemplate.replace("<p>{USER-COMPANY-ADDRESS}</p>", buildReplaceAddress(data.userCompany.address));
   filledTemplate.replace("{USER-COMPANY-EMAIL}", data.userCompany.email);
   filledTemplate.replace("{USER-COMPANY-PHONE}", data.userCompany.phoneNumber);

   filledTemplate.replace("{CLIENT-NAME}", data.clientCompany.name);
   filledTemplate.replace("<p>{CLIENT-ADDRESS}</p>", buildReplaceAddress(data.clientCompany.address));
   filledTemplate.replace("{CLIENT-EMAIL}", data.clientCompany.email);
   filledTemplate.replace("{CLIENT-PHONE}", data.clientCompany.phoneNumber);

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

QString InvoiceDocument::addInternalCss(const QString& htmlContent, const QString& cssContent)
{
   QString styledContent = htmlContent;

   const auto stylesheetStart = htmlContent.indexOf("<link rel=\"stylesheet\"");
   if (stylesheetStart > -1)
   {
      const auto stylesheetEnd = htmlContent.indexOf(">", stylesheetStart);
      const QString internalStylesheet = "<style>\n" + cssContent + "\n</style>";
      styledContent.replace(stylesheetStart, stylesheetEnd-stylesheetStart, internalStylesheet);
   }
   return styledContent;
}

QString InvoiceDocument::buildReplaceAddress(const QString& recordedAddress)
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

QString InvoiceDocument::buildReplaceDetails(const std::vector<InvoiceDetail>& details)
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

QString InvoiceDocument::buildInvoiceTotal(const std::vector<InvoiceDetail>& details)
{
   auto accumulator = [](const double partialSum, const InvoiceDetail& detail)
   {
      return partialSum + detail.value;
   };
   const double totalValue = std::accumulate(details.begin(), details.end(), 0.0, accumulator);
   return QString::asprintf("%.2f", totalValue);
}

QString InvoiceDocument::GetFileFromPattern(const QString& pattern) const
{
   // TODO Add Invoice ID and update doc
   QString filename = pattern;
   filename.replace("[YYYY]", QString::number(invoiceData.date.year()));
   filename.replace("[MM]", QString::asprintf("%02d", invoiceData.date.month()));
   filename.replace("[DD]", QString::asprintf("%02d", invoiceData.date.day()));
   filename.replace("[CLIENT]", invoiceData.clientCompany.name);
   return filename;
}
