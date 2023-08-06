#include "FileResourceAddWidget.h"
#include "ui_FileResourceAddWidget.h"

#include <QFileDialog>

FileResourceAddWidget::FileResourceAddWidget(const QString &fileFilter, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileResourceAddWidget),
    typeDescription(fileFilter)
{
    ui->setupUi(this);

    connect(ui->pathButton, &QAbstractButton::clicked, this, &FileResourceAddWidget::onBrowseClick);
}

FileResourceAddWidget::~FileResourceAddWidget()
{
    delete ui;
}

QString FileResourceAddWidget::getName() const
{
    return ui->nameEdit->text();
}

QString FileResourceAddWidget::getPath() const
{
    return ui->pathEdit->text();
}

void FileResourceAddWidget::onBrowseClick()
{
    const QString title = tr("Select a file");
    QString filePath = QFileDialog::getOpenFileName(this, title, "", typeDescription);
    if (!filePath.isEmpty())
    {
        ui->pathEdit->setText(filePath);
    }
}
