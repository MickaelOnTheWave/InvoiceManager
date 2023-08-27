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

   void setCompanyTitle(const QString& title);

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
    void onRemoveClient(const QModelIndex index);
    void onRemoveTemplate(const QModelIndex index);
    void onRemoveStylesheet(const QModelIndex index);

private:
    void addDataToModel(QWidget* dataWidget, std::function<bool()> insertDataFunc);
    bool canRemoveFileResource(const int id, const QString& dbField) const;
    bool isRemovalConfirmed() const;
    void onRemoveFileResource(const QModelIndex index, FileResourceModel* model,
                              const QString& dbField);
    void onRemoveFromModel(const QModelIndex& index, QAbstractItemModel* model,
                           const QString& dbField,std::function<void(const QModelIndex&)> deleteFunc);

    Ui::MorePage *ui;
    ClientModel* clientModel;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
    InvoiceDbController* controller = nullptr;
};

#endif // MOREPAGE_H
