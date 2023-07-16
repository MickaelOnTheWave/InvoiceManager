#include "DataHandlerWidget.h"
#include "ui_DataHandlerWidget.h"

DataHandlerWidget::DataHandlerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataHandlerWidget)
{
    ui->setupUi(this);

    ui->removeButton->setEnabled(false); // Initially disabled

    // Connect signals and slots
    connect(ui->addButton, &QPushButton::clicked, this, &DataHandlerWidget::onAddClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &DataHandlerWidget::onRemoveClicked);

    // TODO : Finish this and solve warning
    //connect(ui->dataView->selectionModel(), &QItemSelectionModel::currentChanged,
    //        this, &DataHandlerWidget::onSelectionChanged);

    ui->dataView->horizontalHeader()->show();
    ui->dataView->verticalHeader()->hide();
    ui->dataView->setColumnWidth(0, 250);

    ui->dataView->resizeColumnsToContents();
}

DataHandlerWidget::~DataHandlerWidget()
{
    delete ui;
}

void DataHandlerWidget::setColumnsResizingMode(const std::vector<QHeaderView::ResizeMode> &modes)
{
    auto header = ui->dataView->horizontalHeader();
    for (int i=0; i < modes.size(); ++i)
    {
        header->setSectionResizeMode(i, modes[i]);
    }
}

void DataHandlerWidget::setModel(QAbstractItemModel *_model)
{
    model = _model;
    ui->dataView->setModel(model);
    ui->dataView->resizeColumnsToContents();
}

void DataHandlerWidget::resizeEvent(QResizeEvent *event)
{
    //ui->dataView->resizeColumnsToContents();
}

void DataHandlerWidget::onAddClicked()
{
    // Emit the signal when Add button is clicked
    emit addClicked();

    // TODO : Check if really necessary. It might be better to track for model updated event.
    //ui->dataView->update();
    //ui->dataView->resizeColumnsToContents();
}

void DataHandlerWidget::onRemoveClicked()
{
    // Remove the selected item when Remove button is clicked
    QModelIndex currentIndex = ui->dataView->currentIndex();
    if (currentIndex.isValid()) {
        model->removeRow(currentIndex.row());
    }
}

void DataHandlerWidget::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    // Enable/disable Remove button based on item selection
    ui->removeButton->setEnabled(current.isValid());
}

