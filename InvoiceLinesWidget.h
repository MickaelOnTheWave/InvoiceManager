#ifndef INVOICELINESWIDGET_H
#define INVOICELINESWIDGET_H

#include <QWidget>

#include <QVBoxLayout>

class InvoiceLinesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InvoiceLinesWidget(QWidget *parent = nullptr);

public slots:
    void onAddLine();

private:
    using LineData = std::pair<QString, double>;
    QList<LineData> lineData;

    QVBoxLayout* mainLayout;
};

#endif // INVOICELINESWIDGET_H
