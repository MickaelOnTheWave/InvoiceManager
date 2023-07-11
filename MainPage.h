#ifndef MainPage_H
#define MainPage_H

#include <QWidget>

#include <QAbstractItemModel>

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void setCompanyName(const QString& value);

    void connectViewsToModels(QAbstractItemModel* _clientModel,
                              QAbstractItemModel* _stylesheetModel);

signals:
    void closeAndSave();
    void closeAndDiscard();
    void createNewInvoice();
    void goToMore();

private:
    Ui::MainPage *ui;
    QAbstractItemModel* clientModel;
    QAbstractItemModel* stylesheetModel;
};

#endif // MainPage_H
