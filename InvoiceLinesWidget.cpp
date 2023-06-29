#include "InvoiceLinesWidget.h"

InvoiceLinesWidget::InvoiceLinesWidget(QWidget *parent)
    : QWidget{parent}
{
    mainLayout = new QVBoxLayout();


    setLayout(mainLayout);
}

void InvoiceLinesWidget::onAddLine()
{

}
