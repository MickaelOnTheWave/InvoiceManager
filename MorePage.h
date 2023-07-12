#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>

#include <QAbstractItemModel>

#include "ClientModel.h"
#include "StylesheetModel.h"

namespace Ui {
class MorePage;
}

class MorePage : public QWidget
{
    Q_OBJECT

public:
    explicit MorePage(QWidget *parent = nullptr);
    ~MorePage();

    void connectViewsToModels(ClientModel* _clientModel,
                              StylesheetModel* _stylesheetModel);

signals:
    void back();

private slots:
    void onAddClient();
    void onAddStylesheet();

private:
    bool insertInStylesheetModel();

    void addDataToModel(QWidget* dataWidget, std::function<bool()> insertDataFunc);

    Ui::MorePage *ui;
    ClientModel* clientModel;
    StylesheetModel* stylesheetModel;
};

#endif // MOREPAGE_H
