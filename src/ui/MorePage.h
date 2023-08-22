#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>

#include <QAbstractItemModel>

#include "ClientModel.h"
#include "FileResourceModel.h"
#include "InvoiceDbController.h"

namespace Ui {
class MorePage;
}

class MorePage : public QWidget
{
    Q_OBJECT

public:
    explicit MorePage(QWidget *parent = nullptr);
    ~MorePage();

   void setController(InvoiceDbController* _controller);

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
    void onRemoveTemplate(const QModelIndex index);
    void onRemoveStylesheet(const QModelIndex index);

private:
    void addDataToModel(QWidget* dataWidget, std::function<bool()> insertDataFunc);
    bool canRemoveTemplate(const int id) const;
    bool canRemoveStylesheet(const int id) const;
    bool isRemovalConfirmed() const;

    Ui::MorePage *ui;
    ClientModel* clientModel;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
    InvoiceDbController* controller = nullptr;
};

#endif // MOREPAGE_H
