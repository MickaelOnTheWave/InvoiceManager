#ifndef FileResourceAddWidget_H
#define FileResourceAddWidget_H

#include <QWidget>

namespace Ui {
class FileResourceAddWidget;
}

class FileResourceAddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileResourceAddWidget(const QString& fileFilter,
                                   QWidget *parent = nullptr);
    ~FileResourceAddWidget();

    QString getName() const;
    QString getPath() const;

private slots:
    void onBrowseClick();

private:
    Ui::FileResourceAddWidget *ui;
    QString typeDescription;
};

#endif // FileResourceAddWidget_H
