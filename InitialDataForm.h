#ifndef INITIALDATAFORM_H
#define INITIALDATAFORM_H

#include <QWidget>

namespace Ui {
class InitialDataForm;
}

class InitialDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit InitialDataForm(QWidget *parent = nullptr);
    ~InitialDataForm();

signals:
    void confirm();
    void cancel();

private slots:
    void onAddStylesheet();

private:
    Ui::InitialDataForm *ui;
};

#endif // INITIALDATAFORM_H
