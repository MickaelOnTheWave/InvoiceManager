#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>

#include <QAbstractItemModel>

#include "Company.h"

namespace Ui {
class MorePage;
}

class MorePage : public QWidget
{
    Q_OBJECT

public:
    explicit MorePage(QWidget *parent = nullptr);
    ~MorePage();

    void connectViewsToModels(QAbstractItemModel* _clientModel,
                              QAbstractItemModel* _stylesheetModel);

signals:
    void back();

private slots:
    void onAddClient();
    void onAddStylesheet();

private:
    bool insertInClientModel(const CompanyData& data);
    bool insertInStylesheetModel();

    Ui::MorePage *ui;
    QAbstractItemModel* clientModel;
    QAbstractItemModel* stylesheetModel;
};

#endif // MOREPAGE_H
