#include "BackupManager.h"
#include "ZipUtils.h"

BackupManager::BackupManager(const QString &backupBasePath)
    : backupBasePath(backupBasePath) {}

bool BackupManager::backup(const QString &sourceDir) {
    // 创建备份目录
    if (!createBackupDirectory()) {
        return false;
    }
    
    // 生成备份名称
    QString backupName = generateBackupName(sourceDir);
    currentBackupPath = backupBasePath + "/" + backupName;
    
    // 压缩源目录
    QString zipPath = currentBackupPath + "/backup.zip";
    if (!compressDirectory(sourceDir, zipPath)) {
        return false;
    }
    
    // 保存元数据（用于恢复）
    return saveMetadata(sourceDir);
}

bool BackupManager::createBackupDirectory() {
    QDir dir(backupBasePath);
    if (!dir.exists()) {
        return dir.mkpath(".");
    }
    return true;
}

QString BackupManager::generateBackupName(const QString &sourceDir) {
    QDateTime current = QDateTime::currentDateTime();
    QString timestamp = current.toString("yyyyMMdd_hhmmss");
    QString dirName = QDir(sourceDir).dirName();
    return QString("%1_%2").arg(dirName, timestamp);
}

bool BackupManager::compressDirectory(const QString &sourceDir, const QString &zipPath) {
    return ZipUtils::compressDirectory(sourceDir, zipPath);
}

bool BackupManager::saveMetadata(const QString &sourceDir) {
    QFile metaFile(currentBackupPath + "/metadata.json");
    if (!metaFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    QJsonObject metadata;
    metadata["sourcePath"] = sourceDir;
    metadata["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(metadata);
    metaFile.write(doc.toJson());
    metaFile.close();
    
    return true;
}