#ifndef DATABASEFORM_H
#define DATABASEFORM_H

#include <QWidget>

namespace Ui {
class DatabaseForm;
}

class DatabaseForm : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseForm(QWidget *parent = nullptr);
    ~DatabaseForm();

    void setCompanyName(const QString& value);

signals:
    void closeAndSave();
    void closeAndDiscard();
    void createNewInvoice();
    void goToMore();

private:
    Ui::DatabaseForm *ui;
};

#endif // DATABASEFORM_H
