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

    void setDbFile(const QString& dbFile);

signals:
    void closeAndSave();
    void closeAndDiscard();

private slots:
    void onClosePushed();
    void onCloseAndSave();
    void onCloseAndDiscard();
    void onDontClose();

private:
    Ui::DbStatusForm *ui;
};

#endif // DBSTATUSFORM_H
