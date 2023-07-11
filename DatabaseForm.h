#ifndef DATABASEFORM_H
#define DATABASEFORM_H

#include <QWidget>

#include <QAbstractItemModel>

namespace Ui {
class DatabaseForm;
}

class DatabaseForm : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseForm(QWidget *parent = nullptr);
    ~DatabaseForm();

    void setCompanyName(const QString& value);

    void connectViewsToModels(QAbstractItemModel* _clientModel,
                              QAbstractItemModel* _stylesheetModel);

signals:
    void closeAndSave();
    void closeAndDiscard();
    void createNewInvoice();
    void goToMore();

private:
    Ui::DatabaseForm *ui;
    QAbstractItemModel* clientModel;
    QAbstractItemModel* stylesheetModel;
};

#endif // DATABASEFORM_H
