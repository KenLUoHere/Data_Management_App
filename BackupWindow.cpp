#include "BackupWindow.h"
#include "ui_backupwindow.h"
#include <QMessageBox>

BackupWindow::BackupWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackupWindow)
{
    ui->setupUi(this);

    // 连接信号和槽
    connect(ui->browseButton, &QPushButton::clicked, this, &BackupWindow::onBrowseClicked);
    connect(ui->backupButton, &QPushButton::clicked, this, &BackupWindow::onBackupClicked);
}

BackupWindow::~BackupWindow()
{
    delete ui;
}

void BackupWindow::onBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, 
        tr("Select Directory to Backup"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        ui->directoryEdit->setText(dir);
    }
}

void BackupWindow::onBackupClicked()
{
    QString sourceDir = ui->directoryEdit->text();
    if (sourceDir.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Please select a directory to backup."));
        return;
    }

    if (backupManager.backup(sourceDir)) {
        QMessageBox::information(this, tr("Success"), tr("Backup completed successfully."));
        close();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Backup failed."));
    }
} 