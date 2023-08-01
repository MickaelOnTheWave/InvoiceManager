#ifndef NEWINVOICEPAGE_H
#define NEWINVOICEPAGE_H

#include <QWidget>

#include <QComboBox>

#include "ClientModel.h"
#include "InvoiceDbController.h"
#include <QtWebKit>
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
    void onGeneratePreviewClicked();

private:
    void insertTotalRow();
    void computeTotalRow();
    void resetInputData(const QString &companyName);
    void resetInvoiceData();

    void resetComboData(QComboBox* combobox, QAbstractItemModel* model);
    void updateDateEdit(const QDate& date);

    std::vector<InvoiceDetail> createDetailsCollection() const;

    /**
     * @brief writeInvoiceElements
     * @return list of element ids that have been written
     */
    std::vector<int> writeInvoiceElements();

    void addInvoiceDetail(const QString& name, const double value);

    int getComboIndex(QComboBox* combobox, const int id) const;

    QString getCssFile() const;
    QString readTemplateContent() const;
    QString fillTemplate(const QString& templateModel);
    static QString readFileContent(const QString& filename);

    Ui::NewInvoicePage *ui;
    ClientModel* clientModel;
    StylesheetModel* stylesheetModel;
    QAbstractItemModel* invoiceDetailsModel;
    InvoiceDbController* controller;
};

#endif // NEWINVOICEPAGE_H
