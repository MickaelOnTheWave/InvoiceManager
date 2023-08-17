#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>

#include <QAbstractItemModel>

#include "ClientModel.h"
#include "FileResourceModel.h"

namespace Ui {
class MorePage;
}

class MorePage : public QWidget
{
    Q_OBJECT

public:
    explicit MorePage(QWidget *parent = nullptr);
    ~MorePage();

    void connectViewsToModels(ClientModel* _clientModel, FileResourceModel *_templateModel,
                              FileResourceModel* _stylesheetModel);

signals:
    void back();
    void settingsClicked();
    void aboutClicked();

private slots:
    void onAddClient();
    void onAddTemplate();
    void onAddStylesheet();

private:
    void addDataToModel(QWidget* dataWidget, std::function<bool()> insertDataFunc);

    Ui::MorePage *ui;
    ClientModel* clientModel;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
};

#endif // MOREPAGE_H
