#ifndef CHARTSPAGE_H
#define CHARTSPAGE_H

#include <QWidget>

#include <QPieSeries>

#include "InvoiceDbController.h"

namespace Ui {
   class ChartsPage;
}

class ChartsPage : public QWidget
{
   Q_OBJECT

public:
   explicit ChartsPage(QWidget *parent = nullptr);
   ~ChartsPage();

   void setController(InvoiceDbController* _controller);

   void update();

signals:
   void back();

private slots:
   void onPerMonthClicked();
   void onPerYearClicked();

private:
   void setupIncomePerClientViews();
   void setupIncomeTimelineView();
   std::vector<QColor> setupIncomePerClientChart(const InvoiceDbController::IncomePerClientVec& incomePerClient);
   void setupIncomePerClientTable(const InvoiceDbController::IncomePerClientVec& incomePerClient,
                                  const std::vector<QColor>& clientColors);

   void updateAverages(const double timeSpanCount);

   QPieSeries* createClientSeries(const InvoiceDbController::IncomePerClientVec& incomePerClient) const;
   QStringList createTimeLabels(const std::pair<QDate, QDate>& boundaries);

   static QString doubleToStr(const double value);

   Ui::ChartsPage *ui;
   InvoiceDbController* controller;
};

#endif // CHARTSPAGE_H
