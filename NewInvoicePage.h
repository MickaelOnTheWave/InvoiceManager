#ifndef NEWINVOICEPAGE_H
#define NEWINVOICEPAGE_H

#include <QWidget>

#include <QComboBox>
#include "ClientModel.h"
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

    void connectModels(ClientModel* _clientModel,
                       StylesheetModel* _stylesheetModel);
    void reset(const QString &companyName);

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

    Ui::NewInvoicePage *ui;
    ClientModel* clientModel;
    StylesheetModel* stylesheetModel;
    QAbstractItemModel* invoiceDetailsModel;
};

#endif // NEWINVOICEPAGE_H
