#include "InvoiceDocument.h"

#include <QFile>
#include <QPrinter>
#include <QTextDocument>
#include <QTextStream>

void InvoiceDocument::setData(const InvoiceUserData& data)
{
   invoiceData = data;
}

QString InvoiceDocument::CreateHtmlContent()
{
   const QString templateContent = readFileContent(invoiceData.templatePath);
   return fillTemplate(templateContent, invoiceData);
}

void InvoiceDocument::CreatePdfFile(const QString& file)
{
   QTextDocument *document = new QTextDocument();
   document->setHtml(CreateHtmlContent());
   document->setDefaultStyleSheet(invoiceData.stylesheetPath);

   QPrinter printer(QPrinter::HighResolution);
   printer.setPageSize(QPrinter::A4);
   printer.setOutputFormat(QPrinter::PdfFormat);

   printer.setOutputFileName(file);

   document->print(&printer);
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
