#ifndef INITIALDATAFORM_H
#define INITIALDATAFORM_H

#include <QWidget>

#include <QAbstractItemModel>

#include "Company.h"

namespace Ui {
class InitialDataForm;
}

class InitialDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit InitialDataForm(QWidget *parent = nullptr);
    ~InitialDataForm();

    CompanyData getCompanyData() const;

    void setModel(QAbstractItemModel* model);

signals:
    void confirm();
    void cancel();

private slots:
    void onAddStylesheet();

private:
    void insertIntoModel(const QString& stylesheetPath);

    Ui::InitialDataForm *ui;
    QAbstractItemModel* stylesheetModel;
};

#endif // INITIALDATAFORM_H
