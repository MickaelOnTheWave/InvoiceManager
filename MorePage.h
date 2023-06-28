#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>

namespace Ui {
class MorePage;
}

class MorePage : public QWidget
{
    Q_OBJECT

public:
    explicit MorePage(QWidget *parent = nullptr);
    ~MorePage();

private:
    Ui::MorePage *ui;
};

#endif // MOREPAGE_H
