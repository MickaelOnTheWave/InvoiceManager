#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>

#include <QAbstractItemModel>

namespace Ui {
class MorePage;
}

class MorePage : public QWidget
{
    Q_OBJECT

public:
    explicit MorePage(QWidget *parent = nullptr);
    ~MorePage();

    void connectViewsToModels(QAbstractItemModel* clientModel,
                              QAbstractItemModel* stylesheetModel);

signals:
    void back();

private:
    Ui::MorePage *ui;
};

#endif // MOREPAGE_H
