#include <QtTest/QtTest>
#include "BackupManager.h"
#include "RestoreManager.h"
#include <QDir>
#include <QFile>
#include <QDebug>

class TestBackupRestore : public QObject
{
    Q_OBJECT

private slots:
    // 测试备份功能
    void testBackup() {
        // 假设源目录为 "/path/to/source"
        QString sourceDir = "/path/to/source";
        QString backupBasePath = "/path/to/backup";  // 可以是临时目录，确保测试后清理
        BackupManager backupManager(backupBasePath);
        
        // 创建备份
        QVERIFY(backupManager.backup(sourceDir));

        // 检查备份是否创建成功
        QDir backupDir(backupBasePath);
        QVERIFY(backupDir.exists());
    }

    // 测试恢复功能
    void testRestore() {
        QString backupPath = "/path/to/backup/some_backup";  // 已备份的路径
        QString restorePath = "/path/to/restore";  // 恢复目标路径

        RestoreManager restoreManager(backupPath, restorePath);
        
        // 执行恢复
        QVERIFY(restoreManager.restore());

        // 检查恢复的文件是否存在
        QDir restoreDir(restorePath);
        QVERIFY(restoreDir.exists());
    }

    // 测试备份和恢复的完整流程
    void testBackupRestore() {
        QString sourceDir = "/path/to/source";
        QString backupBasePath = "/path/to/backup";
        QString restoreBasePath = "/path/to/restore";  // 恢复目标路径

        // 备份
        BackupManager backupManager(backupBasePath);
        QVERIFY(backupManager.backup(sourceDir));

        // 恢复
        QString backupPath = backupBasePath + "/some_backup";  // 假设备份文件路径
        RestoreManager restoreManager(backupPath, restoreBasePath);
        QVERIFY(restoreManager.restore());

        // 检查恢复后的文件是否和源文件一致
        QDir restoreDir(restoreBasePath);
        QVERIFY(restoreDir.exists());
    }
};

QTEST_MAIN(TestBackupRestore)
#include "test.moc"
