#include "ChartsPage.h"
#include "ui_ChartsPage.h"

#include <QChart>
#include <QPieSeries>
#include <QtCharts/QStackedBarSeries>
#include <QBarSet>

#include <QValueAxis>
#include <QBarCategoryAxis>

ChartsPage::ChartsPage(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::ChartsPage)
{
   ui->setupUi(this);

   connect(ui->backButton, &QAbstractButton::clicked, this, &ChartsPage::back);
   connect(ui->perMonthRadio, &QAbstractButton::clicked, this, &ChartsPage::onPerMonthClicked);
   connect(ui->perYearRadio, &QAbstractButton::clicked, this, &ChartsPage::onPerYearClicked);
}

ChartsPage::~ChartsPage()
{
   delete ui;
}

void ChartsPage::setController(InvoiceDbController* _controller)
{
   controller = _controller;
}

void ChartsPage::update()
{
   ui->totalInvoiceCountLabel->setText(QString::number(controller->getTotalInvoiceCount()));
   ui->clientCountLabel->setText(QString::number(controller->getTotalClientCount()));

   const double amount = controller->getTotalInvoicedAmount();
   const QString amountStr = doubleToStr(amount) + " €";
   ui->totalIncomeLabel->setText(amountStr);

   onPerMonthClicked();

   setupIncomePerClientViews();
   setupIncomeTimelineView();
}

void ChartsPage::onPerMonthClicked()
{
   const int timeSpan = controller->getMonthCount();
   updateAverages(static_cast<double>(timeSpan));
}

void ChartsPage::onPerYearClicked()
{
   const int monthSpan = controller->getMonthCount();
   const double monthsPerYear = 12.0;
   const double yearSpan = monthSpan / monthsPerYear;
   updateAverages(yearSpan);
}

void ChartsPage::setupIncomePerClientViews()
{
   const InvoiceDbController::IncomePerClientVec incomePerClient = controller->getIncomePerClient();
   auto colors = setupIncomePerClientChart(incomePerClient);
   setupIncomePerClientTable(incomePerClient, colors);
}

void ChartsPage::setupIncomeTimelineView()
{
   const IncomeHistory incomeHistory = controller->getIncomeHistory();

   auto incomeTimeSeries = new QStackedBarSeries();
   for (const auto& clientData : incomeHistory)
   {
      auto barSet = new QBarSet(clientData.first);
      for (const double value : clientData.second)
         barSet->append(value);
      incomeTimeSeries->append(barSet);
   }

   const std::pair<QDate, QDate> boundaryMonths = controller->getBoundaryMonths();
   const QStringList timeLabels = createTimeLabels(boundaryMonths);

   QBarCategoryAxis *axisX = new QBarCategoryAxis();
   axisX->append(timeLabels);
   axisX->setLabelsAngle(-90);

   QValueAxis *axisY = new QValueAxis();
   axisY->setMax(incomeHistory.getMaxValue());
   axisY->setTickType(QValueAxis::TicksDynamic);
   axisY->setTickInterval(1000.0);

   incomeTimeSeries->attachAxis(axisX);
   incomeTimeSeries->attachAxis(axisY);


   auto incomeTimeChart = new QChart();
   incomeTimeChart->addSeries(incomeTimeSeries);
   incomeTimeChart->setTitle("Income Timeline");
   incomeTimeChart->legend()->show();
   incomeTimeChart->legend()->setShowToolTips(true);

   incomeTimeChart->addAxis(axisX, Qt::AlignBottom);
   incomeTimeChart->addAxis(axisY, Qt::AlignLeft);

   ui->incomeGraph->setChart(incomeTimeChart);
   ui->incomeGraph->setRenderHint(QPainter::Antialiasing);
}

std::vector<QColor> ChartsPage::setupIncomePerClientChart(const InvoiceDbController::IncomePerClientVec& incomePerClient)
{
   auto pieSeries = createClientSeries(incomePerClient);

   QChart* pieChart = new QChart();
   pieChart->addSeries(pieSeries);
   pieChart->setTitle("Income per Client");
   pieChart->legend()->hide();

   ui->clientsPerChartView->setChart(pieChart);
   ui->clientsPerChartView->setRenderHint(QPainter::Antialiasing);

   std::vector<QColor> colors;
   for(auto slice : pieSeries->slices())
   {
      slice->setExploded();
      colors.push_back(slice->color());
   }
   return colors;
}

void ChartsPage::setupIncomePerClientTable(const InvoiceDbController::IncomePerClientVec& incomePerClient,
                                           const std::vector<QColor>& clientColors)
{
   ui->incomePerClientList->clearContents();
   ui->incomePerClientList->setRowCount(0);
   ui->incomePerClientList->setColumnCount(0);
   ui->incomePerClientList->insertColumn(0);
   ui->incomePerClientList->insertColumn(1);

   auto itClientIncome = incomePerClient.begin();
   auto itColor = clientColors.begin();
   for (; itClientIncome != incomePerClient.end() && itColor != clientColors.end();
        ++itClientIncome, ++itColor)
   {
      const int rowI = ui->incomePerClientList->rowCount();
      ui->incomePerClientList->insertRow(rowI);

      QPixmap pixmap(8, 8);
      pixmap.fill(*itColor);
      auto tableItem = new QTableWidgetItem(QIcon(pixmap), itClientIncome->first);
      tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable);
      ui->incomePerClientList->setItem(rowI, 0, tableItem);

      const QString amount = doubleToStr(itClientIncome->second);
      tableItem = new QTableWidgetItem(amount);
      tableItem->setFlags(tableItem->flags() ^ Qt::ItemIsEditable);
      ui->incomePerClientList->setItem(rowI, 1, tableItem);
   }

   ui->incomePerClientList->verticalHeader()->hide();
   ui->incomePerClientList->horizontalHeader()->hide();

   ui->incomePerClientList->verticalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
   ui->incomePerClientList->verticalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
   ui->incomePerClientList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
   ui->incomePerClientList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void ChartsPage::updateAverages(const double timeSpanCount)
{
   const double spanAsDouble = static_cast<double>(timeSpanCount);
   const double averageInvoiceCount = controller->getTotalInvoiceCount() / spanAsDouble;
   ui->averageInvoiceCountLabel->setText(doubleToStr(averageInvoiceCount));

   const double averageIncome = controller->getTotalInvoicedAmount() / spanAsDouble;
   ui->averageIncomeLabel->setText(doubleToStr(averageIncome) + " €");
}

QPieSeries* ChartsPage::createClientSeries(const InvoiceDbController::IncomePerClientVec& incomePerClient) const
{
   auto series = new QPieSeries();
   for (const auto& clientData : incomePerClient)
      series->append(clientData.first, clientData.second);

   return series;
}

QStringList ChartsPage::createTimeLabels(const std::pair<QDate, QDate>& boundaries)
{
   QStringList labels;

   QDate currentDate(boundaries.first);
   while (currentDate <= boundaries.second && currentDate.isValid())
   {
      labels << currentDate.toString("MMM yyyy");
      currentDate = currentDate.addMonths(1);
   }
   return labels;
}

QString ChartsPage::doubleToStr(const double value)
{
   return QLocale(QLocale::English).toString(value, 'f', 2);
}
