#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backupWindow(nullptr)
    , restoreWindow(nullptr)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onBackupClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onRestoreClicked);
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
        backupWindow = new BackupWindow();
        backupWindow->setWindowFlags(Qt::Window);
        backupWindow->setWindowModality(Qt::ApplicationModal);
        connect(backupWindow, &BackupWindow::destroyed, [this]() {
            backupWindow = nullptr;
        });
    }
    backupWindow->show();
}

void MainWindow::onRestoreClicked()
{
    if (!restoreWindow) {
        restoreWindow = new RestoreWindow();
        restoreWindow->setWindowFlags(Qt::Window);
        restoreWindow->setWindowModality(Qt::ApplicationModal);
        connect(restoreWindow, &RestoreWindow::destroyed, [this]() {
            restoreWindow = nullptr;
        });
    }
    restoreWindow->show();
}
