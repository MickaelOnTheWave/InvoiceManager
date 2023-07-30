#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ClientModel.h"
#include "InvoiceModel.h"
#include "StylesheetModel.h"

#include "InvoiceDbController.h"
#include "DbStatusForm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <QStringListModel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCreateDb();
    void onOpenDb();
    void onCloseDb();

    void onFinishDbCreation();

    void onGoToCreateNewInvoice();
    void onGoToMore();
    void onCreateNewInvoice();
    void onBackToMainPage();


private:
    void createModels();
    void switchToMainWidget();

    void showError(const QString& title, const QString& details);

    bool isDbOpeningConfirmed();

    Ui::MainWindow *ui;

    InvoiceDbController controller;
    ClientModel* clientModel;
    StylesheetModel* stylesheetModel;
    InvoiceModel* invoiceModel;
};
#endif // MAINWINDOW_H
