#ifndef FILERESOURCEDIALOG_H
#define FILERESOURCEDIALOG_H

#include <QDialog>

#include <QAbstractItemModel>

namespace Ui {
   class FileResourceDialog;
}

class FileResourceDialog : public QDialog
{
   Q_OBJECT

public:
   explicit FileResourceDialog(QAbstractItemModel* model, const QModelIndex& index,
                               QWidget *parent = nullptr);
   virtual ~FileResourceDialog();

   QString getData() const;

private slots:
   void onCloseAccept();

private:
   Ui::FileResourceDialog *ui;
   QAbstractItemModel* dataModel;
   QModelIndex dataIndex;
   QString originalData;
};

#endif // FILERESOURCEDIALOG_H
