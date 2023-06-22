#ifndef DATAHANDLERWIDGET_H
#define DATAHANDLERWIDGET_H

#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>

namespace Ui {
class DataHandlerWidget;
}

class DataHandlerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataHandlerWidget(QWidget *parent = nullptr);
    ~DataHandlerWidget();

    void setModel(QAbstractItemModel* _model);

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

signals:
    void addClicked();

private:
    Ui::DataHandlerWidget *ui;
    QAbstractItemModel *model;
};

#endif // DATAHANDLERWIDGET_H
