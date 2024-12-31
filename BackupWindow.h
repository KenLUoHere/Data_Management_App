#ifndef BACKUPWINDOW_H
#define BACKUPWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include "BackupManager.h"

namespace Ui {
class BackupWindow;
}

class BackupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BackupWindow(QWidget *parent = nullptr);
    ~BackupWindow();

private slots:
    void onBrowseClicked();
    void onBackupClicked();

private:
    Ui::BackupWindow *ui;
    BackupManager backupManager;
};

#endif // BACKUPWINDOW_H 