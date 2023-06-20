#include "StylesheedFileForm.h"
#include "ui_StylesheedFileForm.h"

#include <QFileDialog>

StylesheedFileForm::StylesheedFileForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StylesheedFileForm)
{
    ui->setupUi(this);

    connect(ui->fileButton, &QAbstractButton::clicked, this, &StylesheedFileForm::onChooseFile);
}

StylesheedFileForm::~StylesheedFileForm()
{
    delete ui;
}

QString StylesheedFileForm::getFilepath() const
{
    return ui->pathEdit->text();
}

void StylesheedFileForm::onChooseFile()
{
    const QString title = tr("Select a CSS Template file");
    const QString typeDescription = tr("CSS Stylesheet (*.css)");
    const QString cssFile = QFileDialog::getOpenFileName(this, title, QString(), typeDescription);
    if (!cssFile.isEmpty())
    {
        ui->pathEdit->setText(cssFile);
    }
}
