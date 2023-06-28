#ifndef NEWINVOICEPAGE_H
#define NEWINVOICEPAGE_H

#include <QWidget>

namespace Ui {
class NewInvoicePage;
}

class NewInvoicePage : public QWidget
{
    Q_OBJECT

public:
    explicit NewInvoicePage(QWidget *parent = nullptr);
    ~NewInvoicePage();

signals:
    void create();
    void cancel();

private:
    Ui::NewInvoicePage *ui;
};

#endif // NEWINVOICEPAGE_H
