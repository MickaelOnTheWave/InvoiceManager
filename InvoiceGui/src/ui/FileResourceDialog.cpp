#include "FileResourceDialog.h"
#include "ui_FileResourceDialog.h"

FileResourceDialog::FileResourceDialog(QAbstractItemModel* model, const QModelIndex& index,
                                       QWidget* parent) :
   QDialog(parent),
   ui(new Ui::FileResourceDialog),
   dataModel(model)
{
   ui->setupUi(this);

   const int contentColumn = 2;
   dataIndex = model->index(index.row(), contentColumn);
   originalData = model->data(dataIndex).toString();
   ui->textEdit->setPlainText(originalData);

   connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &FileResourceDialog::onCloseAccept);
   connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &FileResourceDialog::close);
}

FileResourceDialog::~FileResourceDialog()
{
   delete ui;
}

QString FileResourceDialog::getData() const
{
   return ui->textEdit->toPlainText();
}

void FileResourceDialog::onCloseAccept()
{
   // TODO send signal to MainPage and handle stuff there.
   // data needs to be written to DB.
   dataModel->setData(dataIndex, ui->textEdit->toPlainText());
   close();
}
