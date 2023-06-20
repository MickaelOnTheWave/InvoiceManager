#ifndef ADDRESSFORM_H
#define ADDRESSFORM_H

#include <QWidget>

namespace Ui {
class AddressForm;
}

class AddressForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddressForm(QWidget *parent = nullptr);
    ~AddressForm();

private:
    Ui::AddressForm *ui;
};

#endif // ADDRESSFORM_H
