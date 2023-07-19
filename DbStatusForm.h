#ifndef DBSTATUSFORM_H
#define DBSTATUSFORM_H

#include <QWidget>

namespace Ui {
class DbStatusForm;
}

class DbStatusForm : public QWidget
{
    Q_OBJECT

public:
    explicit DbStatusForm(QWidget *parent = nullptr);
    ~DbStatusForm();

    void setData(const QString& dbFile, const int dbVersion);

private:
    Ui::DbStatusForm *ui;
};

#endif // DBSTATUSFORM_H
