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

void MorePage::onRemoveTemplate(const QModelIndex index)
{
   const int selectedId = templateModel->data(templateModel->index(index.row(), 0)).toInt();
   if (!canRemoveTemplate(selectedId))
      return;

   if (isRemovalConfirmed())
      templateModel->remove(index);
}

void MorePage::onRemoveStylesheet(const QModelIndex index)
{
   const int selectedId = stylesheetModel->data(stylesheetModel->index(index.row(), 0)).toInt();
   if (!canRemoveStylesheet(selectedId))
      return;

   if (isRemovalConfirmed())
      stylesheetModel->remove(index);
}

bool MorePage::canRemoveTemplate(const int id) const
{
   const int usingCount = controller->getInvoiceCountUsingTemplate(id);
   if (usingCount > 0)
   {
      const QString message = tr("There are %1 invoices using this template. If you wish to remove it, you need to first"
                                 " remove the invoices that are using it.").arg(usingCount);
      QMessageBox::warning(nullptr, "Error", message);
      return false;
   }
   return true;

}

bool MorePage::canRemoveStylesheet(const int id) const
{
   const int usingCount = controller->getInvoiceCountUsingStylesheet(id);
   if (usingCount > 0)
   {
      const QString message = tr("There are %1 invoices using this stylesheet. If you wish to remove it, you need to first"
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
