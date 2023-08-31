#ifndef MainPage_H
#define MainPage_H

#include <QTableView>
#include <QWidget>

#include "ClientModel.h"
#include "InvoiceDbController.h"
#include "InvoiceModel.h"
#include "InvoiceData.h"

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void update();

    void setDisplayData(const QString &companyName, const QString& dbFile,
                        const int dbVersion);

    void setController(InvoiceDbController* _controller);

    void connectViewsToModels(ClientModel* _clientModel,
                              QAbstractItemModel* _templateModel,
                              QAbstractItemModel* _stylesheetModel,
                              InvoiceModel* _invoiceModel);

signals:
    void closeDb();
    void createNewInvoice();
    void createNewInvoiceFromLast();
    void goToMore();
    void settingsClicked();
    void aboutClicked();

private slots:
    void onOpenInvoice(const QModelIndex& index);
    void onOpenClient(const QModelIndex& index);
    void onRemoveInvoice(const int id);
    void onCompanyUpdateCreated();

private:
    void initializeFileResourceView(QTableView* viewControl);
    void setViewDefaults(QTableView* view);

    InvoiceTemplateData createInvoiceTemplateData(const QModelIndex& index);
    CompanyData createCompanyData(const QModelIndex& index);

    Ui::MainPage *ui;
    ClientModel* clientModel;
    QAbstractItemModel* templateModel;
    QAbstractItemModel* stylesheetModel;
    InvoiceModel* invoiceModel;
    InvoiceDbController* controller = nullptr;
};

#endif // MainPage_H
