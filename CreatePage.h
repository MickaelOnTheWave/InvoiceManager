#ifndef CreatePage_H
#define CreatePage_H

#include <QWidget>

#include <QAbstractItemModel>

#include "Company.h"

namespace Ui {
class CreatePage;
}

class CreatePage : public QWidget
{
    Q_OBJECT

public:
    explicit CreatePage(QWidget *parent = nullptr);
    ~CreatePage();

    CompanyData getCompanyData() const;

    void setModel(QAbstractItemModel* model);

signals:
    void confirm();
    void cancel();

private slots:
    void onAddStylesheet();

private:
    void insertIntoModel(const QString& stylesheetPath);

    Ui::CreatePage *ui;
    QAbstractItemModel* stylesheetModel;
};

#endif // CreatePage_H
