#include "StylesheetDetailsWidget.h"
#include "ui_StylesheetDetailsWidget.h"

#include <QFileDialog>

StylesheetDetailsWidget::StylesheetDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StylesheetDetailsWidget)
{
    ui->setupUi(this);

    connect(ui->pathButton, &QAbstractButton::clicked, this, &StylesheetDetailsWidget::onBrowseClick);
}

StylesheetDetailsWidget::~StylesheetDetailsWidget()
{
    delete ui;
}

QString StylesheetDetailsWidget::getPath() const
{
    return ui->pathEdit->text();
}

void StylesheetDetailsWidget::onBrowseClick()
{
    const QString title = tr("Select an Invoice Stylesheet");
    const QString typeDescription = tr("Stylesheet (*.css)");
    QString filePath = QFileDialog::getOpenFileName(this, title, "", typeDescription);
    if (!filePath.isEmpty())
    {
        ui->pathEdit->setText(filePath);
    }
}
