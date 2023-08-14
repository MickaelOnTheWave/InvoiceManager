#ifndef CreatePage_H
#define CreatePage_H

#include <QWidget>

#include "CompanyData.h"
#include "FileResourceModel.h"

namespace Ui {
class CreatePage;
}

class CreatePage : public QWidget
{
    Q_OBJECT

public:
    explicit CreatePage(QWidget *parent = nullptr);
    ~CreatePage();

    void reset();

    CompanyData getCompanyData() const;

    void setModel(FileResourceModel* _templateModel,
                  FileResourceModel* _stylesheetModel);

signals:
    void confirm();
    void cancel();

private slots:
    void onAddStylesheet();
    void onAddTemplate();

private:
    Ui::CreatePage *ui;
    FileResourceModel* templateModel;
    FileResourceModel* stylesheetModel;
};

#endif // CreatePage_H
