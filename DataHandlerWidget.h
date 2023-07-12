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

signals:
    void addClicked();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::DataHandlerWidget *ui;
    QAbstractItemModel *model;
};

#endif // DATAHANDLERWIDGET_H
