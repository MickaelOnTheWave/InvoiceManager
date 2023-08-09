#ifndef MainPage_H
#define MainPage_H

#include <QTableView>
#include <QWidget>

#include "ClientModel.h"
#include "InvoiceModel.h"

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void setDisplayData(const QString &companyName, const QString& dbFile,
                        const int dbVersion);

    void connectViewsToModels(ClientModel* _clientModel,
                              QAbstractItemModel* _templateModel,
                              QAbstractItemModel* _stylesheetModel,
                              InvoiceModel* _invoiceModel);

signals:
    void closeDb();
    void createNewInvoice();
    void createNewInvoiceFromLast();
    void goToMore();

private slots:
    void onOpenInvoice(const QModelIndex& index);

private:
    void initializeFileResourceView(QTableView* viewControl);
    void setViewDefaults(QTableView* view);

    Ui::MainPage *ui;
    ClientModel* clientModel;
    QAbstractItemModel* templateModel;
    QAbstractItemModel* stylesheetModel;
    InvoiceModel* invoiceModel;
};

#endif // MainPage_H
