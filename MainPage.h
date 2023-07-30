#ifndef MainPage_H
#define MainPage_H

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
                              QAbstractItemModel* _stylesheetModel,
                              InvoiceModel* _invoiceModel);

signals:
    void closeDb();
    void createNewInvoice();
    void goToMore();

private:
    Ui::MainPage *ui;
    ClientModel* clientModel;
    QAbstractItemModel* stylesheetModel;
    InvoiceModel* invoiceModel;
};

#endif // MainPage_H
