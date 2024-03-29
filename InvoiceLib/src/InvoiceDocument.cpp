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
#include "WkPdfConverter.h"

void InvoiceDocument::setData(const InvoiceUserData& data)
{
   invoiceData = data;
}

QString InvoiceDocument::CreateHtmlContent() const
{
   return fillTemplate(invoiceData.templateData, invoiceData);
}

QString InvoiceDocument::CreateStyledHtmlContent() const
{
   const QString htmlContent = CreateHtmlContent();
   return addInternalCss(htmlContent, invoiceData.stylesheetData);
}

bool InvoiceDocument::CreateHtmlFile(const QString& filename)
{
   const QString styledHtmlContent = CreateStyledHtmlContent();
   QFile f(filename);
   if (!f.open(QIODevice::ReadWrite))
       return false;

   QTextStream stream(&f);
   stream << styledHtmlContent << Qt::endl;
   return true;
}

bool InvoiceDocument::CreatePdfFile(const QString& filename)
{
   const QString styledHtmlContent = CreateStyledHtmlContent();
   WkPdfConverter converter;
   const bool ok = converter.ConvertHtmlToFile(styledHtmlContent, filename);
   return ok;
}

bool InvoiceDocument::CreatePdfFileFromPattern(const QString& filenamePattern)
{
   const bool ok = CreatePdfFile(GetFileFromPattern(filenamePattern));
   return ok;
}

int InvoiceDocument::GetInvoiceId() const
{
   return invoiceData.id;
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

   filledTemplate.replace("<tr>{INVOICE-DETAILS}</tr>", buildReplaceDetailsV8(data.details));
   replaceNewerDetails(filledTemplate, data.details);
   filledTemplate.replace("{INVOICE-TOTAL}", buildInvoiceTotal(data.details));

   filledTemplate.replace("{CURRENCY}", data.currency);
   filledTemplate.replace("{NOTES}", data.notes);

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

QString InvoiceDocument::buildReplaceDetailsV8(const std::vector<InvoiceDetail>& details)
{
   QString replacedStr;
   for (const auto& detailLine : details)
   {
       QString valueStr = QString::asprintf("%.2f", detailLine.value * detailLine.quantity);

       const QString nameCell = QString("\t\t\t<td>%1</td>").arg(detailLine.description);
       const QString valueCell = QString("\t\t\t<td>%1</td>").arg(valueStr);
       replacedStr += QString("<tr>\n%1\n%2\n\t\t</tr>\n").arg(nameCell, valueCell);
   }
   return replacedStr;
}

void InvoiceDocument::replaceNewerDetails(QString& templateData, const std::vector<InvoiceDetail>& details)
{
   const QString startSectionTag = "<tr>{INVOICE-DETAILS}";
   const QString endSectionTag = "</tr>";
   int startIndex = 0;
   while (startIndex > -1)
   {
      startIndex = templateData.indexOf(startSectionTag);
      const int endIndex = templateData.indexOf("</tr>", startIndex);
      if (endIndex < 0)
         // Malformed template, better to do nothing.
         break;

      const QString oldDetailsSection = templateData.mid(startIndex, endIndex-startIndex+endSectionTag.length());

      const int innerSectionLength = oldDetailsSection.length() - startSectionTag.length() - endSectionTag.length();
      const QString innerDetailSection = oldDetailsSection.mid(startSectionTag.length(), innerSectionLength).trimmed();


      QString replacedStr;
      for (const auto& detailLine : details)
      {
         const QString descriptionStr = QString("<td>") + detailLine.description + "</td>";
         const QString valueStr = QString::asprintf("<td>%.2f</td>", detailLine.value);
         const QString quantityStr = QString::asprintf("<td>%.2f</td>", detailLine.quantity);
         const QString totalStr = QString::asprintf("<td>%.2f</td>", detailLine.value * detailLine.quantity);

         QString innerSectionInstance = innerDetailSection;
         innerSectionInstance.replace("<td>{DESCRIPTION}</td>", descriptionStr);
         innerSectionInstance.replace("<td>{QUANTITY}</td>", quantityStr);
         innerSectionInstance.replace("<td>{UNITVALUE}</td>", valueStr);
         innerSectionInstance.replace("<td>{TOTAL}</td>", totalStr);

         replacedStr += "\n\n<tr>\n";
         replacedStr += innerSectionInstance;
         replacedStr += "\n\n</tr>\n";
      }

      templateData.replace(oldDetailsSection, replacedStr);
   }
}

QString InvoiceDocument::buildInvoiceTotal(const std::vector<InvoiceDetail>& details)
{
   auto accumulator = [](const double partialSum, const InvoiceDetail& detail)
   {
      return partialSum + (detail.quantity * detail.value);
   };
   const double totalValue = std::accumulate(details.begin(), details.end(), 0.0, accumulator);
   return QString::asprintf("%.2f", totalValue);
}

QString InvoiceDocument::GetFileFromPattern(const QString& pattern) const
{
   QString filename = pattern;
   filename.replace("[YYYY]", QString::number(invoiceData.date.year()));
   filename.replace("[MM]", QString::asprintf("%02d", invoiceData.date.month()));
   filename.replace("[DD]", QString::asprintf("%02d", invoiceData.date.day()));
   filename.replace("[CLIENT]", invoiceData.clientCompany.name);
   filename.replace("[ID]", QString::number(invoiceData.id));
   return filename;
}
