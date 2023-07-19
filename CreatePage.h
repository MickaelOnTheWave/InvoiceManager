#ifndef CreatePage_H
#define CreatePage_H

#include <QWidget>

#include "Company.h"
#include "StylesheetModel.h"

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

    void setModel(StylesheetModel* model);

signals:
    void confirm();
    void cancel();

private slots:
    void onAddStylesheet();

private:
    Ui::CreatePage *ui;
    StylesheetModel* stylesheetModel;
};

#endif // CreatePage_H
