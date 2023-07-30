#ifndef DATAHANDLERWIDGET_H
#define DATAHANDLERWIDGET_H

#include <vector>

#include <QHeaderView>
#include <QItemDelegate>
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

    void hideColumns(const std::vector<int>& columns);
    void setColumnsResizingMode(const std::vector<QHeaderView::ResizeMode> &modes);
    void setModel(QAbstractItemModel* _model);

signals:
    void addClicked();
    void editingFinished();

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

class DataViewDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit DataViewDelegate(QObject *parent = nullptr);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void editingFinished() const;
};

#endif
