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
    void refreshData(const QString &companyName);

signals:
    void create();
    void cancel();

private slots:
    void onClientComboChange(int index);

private:
    QStringList buildClientNames() const;

    void resetComboData(QComboBox* combo, const QStringList& newData);

    Ui::NewInvoicePage *ui;
    ClientModel* clientModel;
    StylesheetModel* stylesheetModel;
};

#endif // NEWINVOICEPAGE_H
