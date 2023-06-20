#ifndef STYLESHEEDFILEFORM_H
#define STYLESHEEDFILEFORM_H

#include <QWidget>

namespace Ui {
class StylesheedFileForm;
}

class StylesheedFileForm : public QWidget
{
    Q_OBJECT

public:
    explicit StylesheedFileForm(QWidget *parent = nullptr);
    ~StylesheedFileForm();

    QString getFilepath() const;

private slots:
    void onChooseFile();

private:
    Ui::StylesheedFileForm *ui;
};

#endif // STYLESHEEDFILEFORM_H
