#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backupWindow(nullptr)
    , restoreWindow(nullptr)
{
    ui->setupUi(this);

    connect(ui->backupButton, &QPushButton::clicked, this, &MainWindow::onBackupClicked);
    connect(ui->restoreButton, &QPushButton::clicked, this, &MainWindow::onRestoreClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (backupWindow) {
        delete backupWindow;
    }
    if (restoreWindow) {
        delete restoreWindow;
    }
}

void MainWindow::onBackupClicked()
{
    if (!backupWindow) {
        backupWindow = new BackupWindow(this);
    }
    backupWindow->show();
}

void MainWindow::onRestoreClicked()
{
    if (!restoreWindow) {
        restoreWindow = new RestoreWindow(this);
    }
    restoreWindow->show();
}
