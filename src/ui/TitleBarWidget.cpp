#include "TitleBarWidget.h"
#include "ui_TitleBarWidget.h"

TitleBarWidget::TitleBarWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::TitleBarWidget)
{
   ui->setupUi(this);

   connect(ui->settingsButton, &QAbstractButton::clicked, this, &TitleBarWidget::settingsClicked);
   connect(ui->aboutButton, &QAbstractButton::clicked, this, &TitleBarWidget::aboutClicked);
}

TitleBarWidget::~TitleBarWidget()
{
   delete ui;
}

void TitleBarWidget::setTitle(const QString& title)
{
   ui->titleLabel->setText(title);
}
