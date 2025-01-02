#include "RestoreWindow.h"
#include "ui_restorewindow.h"
#include <QMessageBox>
#include <QDir>

RestoreWindow::RestoreWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestoreWindow)
{
    ui->setupUi(this);
    
    // 连接信号和槽
    connect(ui->browseButton, &QPushButton::clicked, this, &RestoreWindow::onBrowseClicked);
    connect(ui->restoreButton, &QPushButton::clicked, this, &RestoreWindow::onRestoreClicked);
    
    // 更新备份列表
    updateBackupList();
}

RestoreWindow::~RestoreWindow()
{
    delete ui;
}

void RestoreWindow::onBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, 
        tr("Select Restore Directory"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        ui->restorePathEdit->setText(dir);
    }
}

void RestoreWindow::onRestoreClicked()
{
    if (ui->backupList->currentItem() == nullptr) {
        QMessageBox::warning(this, tr("Error"), tr("Please select a backup to restore."));
        return;
    }

    QString backupPath = "/root/backup/" + ui->backupList->currentItem()->text();
    QString restorePath = ui->restorePathEdit->text();
    
    restoreManager = new RestoreManager(backupPath, restorePath);
    
    if (restoreManager->restore()) {
        QMessageBox::information(this, tr("Success"), tr("Restore completed successfully."));
        close();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Restore failed."));
    }
    
    delete restoreManager;
    restoreManager = nullptr;
}

void RestoreWindow::updateBackupList()
{
    QDir backupDir("/root/backup");
    if (!backupDir.exists()) {
        qWarning() << "Backup directory does not exist:" << backupDir.absolutePath();
        return;
    }

    QStringList backups = backupDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    ui->backupList->clear();
    ui->backupList->addItems(backups);
} 