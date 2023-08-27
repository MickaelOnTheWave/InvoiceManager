#ifndef COMPANYDETAILSWIDGET_H
#define COMPANYDETAILSWIDGET_H

#include <QWidget>

#include <QLineEdit>
#include "src/CompanyData.h"

namespace Ui {
class CompanyDetailsWidget;
}

class CompanyDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompanyDetailsWidget(QWidget *parent = nullptr);
    ~CompanyDetailsWidget();

    CompanyData getData() const;
    void fill(const CompanyData& data);
    void reset();

signals:
    void dataChanged();

private:
    void connectTextEditChanges();

    QString buildAddress() const;

    static void addAddressLine(QString& finalAddress, const QString& addressLine);
    static QString getExplodedValue(const QStringList& exploded, const int index);

    Ui::CompanyDetailsWidget *ui;
    std::vector<QLineEdit*> dataControls;
};

#endif // COMPANYDETAILSWIDGET_H
