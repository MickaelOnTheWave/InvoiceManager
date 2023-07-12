#ifndef NEWINVOICEPAGE_H
#define NEWINVOICEPAGE_H

#include <QWidget>

#include "ClientModel.h"

namespace Ui {
class NewInvoicePage;
}

class NewInvoicePage : public QWidget
{
    Q_OBJECT

public:
    explicit NewInvoicePage(QWidget *parent = nullptr);
    ~NewInvoicePage();

    void connectModels(ClientModel* _clientModel);
    void refreshData();

signals:
    void create();
    void cancel();

private slots:
    void onClientComboChange(int index);

private:
    QStringList buildNames() const;

    Ui::NewInvoicePage *ui;
    ClientModel* clientModel;
};

#endif // NEWINVOICEPAGE_H
