/*
 * InvoiceManager
 * Copyright (C) 2023 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DataHandlerWidget.h"
#include "ui_DataHandlerWidget.h"

DataHandlerWidget::DataHandlerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataHandlerWidget)
{
    ui->setupUi(this);

    ui->removeButton->setEnabled(false); // Initially disabled

    // Connect signals and slots
    connect(ui->addButton, &QPushButton::clicked, this, &DataHandlerWidget::addClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &DataHandlerWidget::onRemoveClicked);

    ui->dataView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->dataView->horizontalHeader()->show();
    ui->dataView->verticalHeader()->hide();
    ui->dataView->setColumnWidth(0, 250);

    ui->dataView->resizeColumnsToContents();

    auto detectionDelegate = new DataViewDelegate(this);
    ui->dataView->setItemDelegate(detectionDelegate);
    connect(detectionDelegate, &DataViewDelegate::editingFinished,
            this, &DataHandlerWidget::editingFinished);
}

DataHandlerWidget::~DataHandlerWidget()
{
   delete ui;
}

void DataHandlerWidget::setupTotalSpan()
{
   auto model = ui->dataView->model();
   if (model)
      ui->dataView->setSpan(model->rowCount()-1, 0, 1, 3);
}

void DataHandlerWidget::hideColumns(const std::vector<int> &columns)
{
    for (const int col : columns)
        ui->dataView->hideColumn(col);
}

void DataHandlerWidget::setColumnsResizingMode(const std::vector<QHeaderView::ResizeMode> &modes)
{
    auto header = ui->dataView->horizontalHeader();
    for (int i=0; i < modes.size(); ++i)
        header->setSectionResizeMode(i, modes[i]);
}

void DataHandlerWidget::setColumnsSizes(const std::vector<int> &sizes)
{
    for (int i=0; i < sizes.size(); ++i)
    {
        const int colSize = sizes[i];
        if (colSize > -1)
            ui->dataView->setColumnWidth(i, colSize);
    }
}

void DataHandlerWidget::setModel(QAbstractItemModel *_model)
{
    model = _model;
    ui->dataView->setModel(model);
    ui->dataView->resizeColumnsToContents();

    connect(ui->dataView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &DataHandlerWidget::onSelectionChanged);
}

void DataHandlerWidget::resizeEvent(QResizeEvent *event)
{
    //ui->dataView->resizeColumnsToContents();
}

void DataHandlerWidget::onRemoveClicked()
{
   emit removeClicked(ui->dataView->currentIndex());
}

void DataHandlerWidget::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    // Enable/disable Remove button based on item selection
    ui->removeButton->setEnabled(current.isValid());
}


DataViewDelegate::DataViewDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void DataViewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QItemDelegate::setModelData(editor, model, index);
    emit editingFinished();
}

QWidget *DataViewDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int lastRow = index.model()->rowCount() - 1;
    if (index.row() == lastRow && index.column() == 1)
        return nullptr;

    return QItemDelegate::createEditor(parent, option, index);
}
