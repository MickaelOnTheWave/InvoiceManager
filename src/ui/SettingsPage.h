#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

namespace Ui {
   class SettingsPage;
}

class SettingsPage : public QWidget
{
   Q_OBJECT

public:
   explicit SettingsPage(QWidget *parent = nullptr);
   ~SettingsPage();

   void refresh();

signals:
   void done();

private slots:
   void onReset();
   void onSave();

private:
   void loadSettings();
   void saveSettings();

   Ui::SettingsPage *ui;
};

#endif // SETTINGSPAGE_H
