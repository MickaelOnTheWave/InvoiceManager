#ifndef INITIALDATAFORM_H
#define INITIALDATAFORM_H

#include <QWidget>

#include <QAbstractItemModel>

namespace Ui {
class InitialDataForm;
}

class InitialDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit InitialDataForm(QWidget *parent = nullptr);
    ~InitialDataForm();

    void setModel(QAbstractItemModel* model);
    QString getCompanyName() const;
    QString getCompanyAddress() const;
    QString getCompanyEmail() const;

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
