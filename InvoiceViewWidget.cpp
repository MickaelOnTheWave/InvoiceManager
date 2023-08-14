#include "InvoiceViewWidget.h"
#include "ui_InvoiceViewWidget.h"

#include <QFile>

InvoiceViewWidget::InvoiceViewWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::InvoiceViewWidget)
{
   ui->setupUi(this);
}

InvoiceViewWidget::~InvoiceViewWidget()
{
   delete ui;
}

void InvoiceViewWidget::show(const InvoiceTemplateData& data)
{
   const QString templateContent = readFileContent(data.templatePath);
   const QString previewContent = fillTemplate(templateContent, data);

   const QUrl cssUrl = QUrl::fromLocalFile(data.stylesheetPath);
   ui->webView->settings()->setUserStyleSheetUrl(cssUrl);
   ui->webView->setHtml(previewContent);
   ui->webView->show();
}


QString InvoiceViewWidget::readFileContent(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return QString();
    QTextStream in(&f);
    return in.readAll();
}

QString InvoiceViewWidget::fillTemplate(const QString& templateModel,
                                        const InvoiceTemplateData& data)
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

QString InvoiceViewWidget::buildReplaceAddress(const QString& recordedAddress)
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

QString InvoiceViewWidget::buildReplaceDetails(const std::vector<InvoiceDetail>& details)
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

QString InvoiceViewWidget::buildInvoiceTotal(const std::vector<InvoiceDetail>& details)
{
   auto accumulator = [](const double partialSum, const InvoiceDetail& detail)
   {
      return partialSum + detail.value;
   };
   const double totalValue = std::accumulate(details.begin(), details.end(), 0.0, accumulator);
   return QString::asprintf("%.2f", totalValue);
}
