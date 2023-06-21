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

private:
    Ui::DatabaseForm *ui;
};

#endif // DATABASEFORM_H
