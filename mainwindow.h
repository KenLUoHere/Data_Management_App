#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BackupWindow.h"
#include "RestoreWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBackupClicked();
    void onRestoreClicked();

private:
    Ui::MainWindow *ui;
    BackupWindow *backupWindow;
    RestoreWindow *restoreWindow;
};
#endif // MAINWINDOW_H
