#include "MorePage.h"
#include "ui_MorePage.h"

#include <QMessageBox>
#include <QSettings>

#include "CompanyData.h"
#include "CompanyDetailsWidget.h"
#include "GuiUtils.h"
#include "FileResourceAddWidget.h"
#include "NewDataDialog.h"

MorePage::MorePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorePage)
{
    ui->setupUi(this);

    connect(ui->clientsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddClient);
    connect(ui->clientsWidget, &DataHandlerWidget::removeClicked, this, &MorePage::onRemoveClient);
    connect(ui->templatesWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddTemplate);
    connect(ui->templatesWidget, &DataHandlerWidget::removeClicked, this, &MorePage::onRemoveTemplate);
    connect(ui->stylesheetsWidget, &DataHandlerWidget::addClicked, this, &MorePage::onAddStylesheet);
    connect(ui->stylesheetsWidget, &DataHandlerWidget::removeClicked, this, &MorePage::onRemoveStylesheet);
    connect(ui->backButton, &QAbstractButton::clicked, this, &MorePage::back);

    connect(ui->titleBarWidget, &TitleBarWidget::settingsClicked, this, &MorePage::settingsClicked);
    connect(ui->titleBarWidget, &TitleBarWidget::aboutClicked, this, &MorePage::aboutClicked);
}

MorePage::~MorePage()
{
   delete ui;
}

void MorePage::setCompanyTitle(const QString& title)
{
   ui->titleBarWidget->setTitle(title);
}

void MorePage::setController(InvoiceDbController* _controller)
{
   controller = _controller;
}

void MorePage::connectViewsToModels(ClientModel *_clientModel,
                                    FileResourceModel *_templateModel,
                                    FileResourceModel *_stylesheetModel)
{
    ui->clientsWidget->setModel(_clientModel);
    ui->templatesWidget->setModel(_templateModel);
    ui->stylesheetsWidget->setModel(_stylesheetModel);

    ui->clientsWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Stretch, QHeaderView::Stretch});
    ui->templatesWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch});
    ui->stylesheetsWidget->setColumnsResizingMode({QHeaderView::Fixed, QHeaderView::Fixed, QHeaderView::Stretch});

    clientModel = _clientModel;
    templateModel = _templateModel;
    stylesheetModel = _stylesheetModel;

    ui->clientsWidget->hideColumns({0, 2, 5});
}

void MorePage::onAddClient()
{
    auto contentWidget = new CompanyDetailsWidget();
    GuiUtils::addDataToModel(contentWidget, tr("New Client"), [this, contentWidget] () {
        return clientModel->insertAtEnd(contentWidget->getData());
    });
}

void MorePage::onAddTemplate()
{
    GuiUtils::OnAddTemplate(templateModel);
}

void MorePage::onAddStylesheet()
{
   GuiUtils::OnAddStylesheet(stylesheetModel);
}

void MorePage::onRemoveClient(const QModelIndex index)
{
   auto deleter = [this] (const QModelIndex& index) {
      clientModel->remove(index);
   };
   onRemoveFromModel(index, clientModel, "clientId", deleter);
}

void MorePage::onRemoveTemplate(const QModelIndex index)
{
   onRemoveFileResource(index, templateModel, "templateId");
}

void MorePage::onRemoveStylesheet(const QModelIndex index)
{
   onRemoveFileResource(index, stylesheetModel, "stylesheetId");
}

bool MorePage::canRemoveFileResource(const int id, const QString& dbField) const
{
   const int usingCount = controller->getInvoiceCountUsingFile(id, dbField);
   if (usingCount > 0)
   {
      const QString message = tr("There are %1 invoices using this resource. If you wish to remove it, you need to first"
                                 " remove the invoices that are using it.").arg(usingCount);
      QMessageBox::warning(nullptr, "Error", message);
      return false;
   }
   return true;
}

bool MorePage::isRemovalConfirmed() const
{
   QSettings settings;
   const bool confirmRemoval = settings.value("user/removeconfirmation", true).toBool();

   if (confirmRemoval)
   {
      // TODO : Create dialog with a "Don't ask me again" checkbox.
      const auto selectedButton = QMessageBox::question(nullptr, "Removal", "Are you sure?");
      return (selectedButton == QMessageBox::Yes);
   }
   return true;
}

void MorePage::onRemoveFileResource(const QModelIndex index, FileResourceModel* model,
                                    const QString& dbField)
{
   auto deleter = [model](const QModelIndex& index) {
      model->remove(index);
   };
   onRemoveFromModel(index, model, dbField, deleter);
}

void MorePage::onRemoveFromModel(const QModelIndex& index, QAbstractItemModel* model, const QString& dbField,
                                 std::function<void (const QModelIndex&)> deleteFunc)
{
   const int selectedId = model->data(model->index(index.row(), 0)).toInt();
   if (!canRemoveFileResource(selectedId, dbField))
      return;

   if (isRemovalConfirmed())
      deleteFunc(index);
}
