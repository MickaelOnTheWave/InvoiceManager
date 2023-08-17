#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ClientModel.h"
#include "InvoiceModel.h"
#include "FileResourceModel.h"

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
    void onOpenLastDb();
    void onCloseDb();
    void onSettings();
    void onAbout();

    void onFinishDbCreation();

    void onGoToCreateNewInvoice();
    void onGoToCreateNewInvoiceFromLast();
    void onGoToMore();
    void onCreateNewInvoice();
    void onBackToMainPage();
    void onBackToPreviousPage();


private:
    void initializeOpenLastDb();
    void createModels();
    void switchToMainWidget();

    void showError(const QString& title, const QString& details);

    bool isDbOpeningConfirmed();

    void openDb(const QString& file);

    Ui::MainWindow *ui;

    InvoiceDbController controller;
    ClientModel* clientModel;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
    InvoiceModel* invoiceModel;
    QWidget* previousPage;
};
#endif // MAINWINDOW_H
