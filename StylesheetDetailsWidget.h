#ifndef STYLESHEETDETAILSWIDGET_H
#define STYLESHEETDETAILSWIDGET_H

#include <QWidget>

namespace Ui {
class StylesheetDetailsWidget;
}

class StylesheetDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StylesheetDetailsWidget(QWidget *parent = nullptr);
    ~StylesheetDetailsWidget();

    QString getPath() const;

private slots:
    void onBrowseClick();

private:
    Ui::StylesheetDetailsWidget *ui;
};

#endif // STYLESHEETDETAILSWIDGET_H
