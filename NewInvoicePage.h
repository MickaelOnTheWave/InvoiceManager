#ifndef NEWINVOICEPAGE_H
#define NEWINVOICEPAGE_H

#include <QWidget>

#include <QComboBox>

#include "ClientModel.h"
#include "InvoiceDbController.h"
#include "StylesheetModel.h"

namespace Ui {
class NewInvoicePage;
}

class NewInvoicePage : public QWidget
{
    Q_OBJECT

public:
    explicit NewInvoicePage(QWidget *parent = nullptr);
    ~NewInvoicePage();

    void initialize(ClientModel* _clientModel,
                    StylesheetModel* _stylesheetModel,
                    InvoiceDbController* _controller);
    void reset();
    void resetFromLast();

signals:
    void create();
    void cancel();

private slots:
    void onClientComboChange(int index);
    void onAddInvoiceDetail();
    void onCreateInvoice();

    void onTodayClicked();
    void onLastDayOfMonthClicked();
    void onCustomDateClicked();
    void onCustomDateUpdated(const QDate& newDate);

private:
    void insertTotalRow();
    void computeTotalRow();
    void resetInputData(const QString &companyName);
    void resetInvoiceData();

    QStringList buildClientNames() const;

    void resetComboData(QComboBox* combo, const QStringList& newData);
    void updateDateEdit(const QDate& date);


    std::vector<InvoiceDetail> createDetailsCollection() const;

    /**
     * @brief writeInvoiceElements
     * @return list of element ids that have been written
     */
    std::vector<int> writeInvoiceElements();

    void addInvoiceDetail(const QString& name, const double value);

    static int getClientIndex(const int id);
    static int getStylesheetIndex(const int id);

    Ui::NewInvoicePage *ui;
    ClientModel* clientModel;
    StylesheetModel* stylesheetModel;
    QAbstractItemModel* invoiceDetailsModel;
    InvoiceDbController* controller;
};

#endif // NEWINVOICEPAGE_H
