#ifndef NEWDATADIALOG_H
#define NEWDATADIALOG_H

#include <QDialog>

namespace Ui {
class NewDataDialog;
}

class NewDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDataDialog(QWidget *contentWidget, QWidget *parent = nullptr);
    ~NewDataDialog();

private:
    Ui::NewDataDialog *ui;
};

#endif // NEWDATADIALOG_H
