#ifndef INVOICEVIEWWIDGET_H
#define INVOICEVIEWWIDGET_H

#include <QWidget>

#include "InvoiceModule/include/InvoiceData.h"

namespace Ui {
   class InvoiceViewWidget;
}

class InvoiceViewWidget : public QWidget
{
   Q_OBJECT

public:
   explicit InvoiceViewWidget(QWidget *parent = nullptr);
   ~InvoiceViewWidget();

   void show(const InvoiceTemplateData& data);

private:
   static QString readFileContent(const QString& filename);
   static QString fillTemplate(const QString& templateModel,
                               const InvoiceTemplateData& data);

   static QString buildReplaceAddress(const QString& recordedAddress);
   static QString buildReplaceDetails(const std::vector<InvoiceDetail>& details);
   static QString buildInvoiceTotal(const std::vector<InvoiceDetail>& details);


   Ui::InvoiceViewWidget *ui;
};

#endif // INVOICEVIEWWIDGET_H
