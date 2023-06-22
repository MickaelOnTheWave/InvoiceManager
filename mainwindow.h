#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void onCloseAndSaveDb();
    void onCloseAndDiscardDb();

    void onFinishDbCreation();


private:
    void connectDbStatusControls(DbStatusForm* dbStatusForm);
    void createModels();

    Ui::MainWindow *ui;

    InvoiceDbController controller;
    QStringListModel* stylesheetModel;
};
#endif // MAINWINDOW_H
