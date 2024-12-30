#include "BackupManager.h"

BackupManager::BackupManager(const QString &backupBasePath)
    : backupBasePath(backupBasePath) {}

BackupManager::~BackupManager() {}

bool BackupManager::backup(const QString &sourceDir) {
    // Create backup directory
    if (!createBackupDirectory()) {
        qWarning() << "Backup failed: Cannot create backup directory" << backupBasePath;
        return false;
    }
    
    // Generate backup name
    QString backupName = generateBackupName(sourceDir);
    currentBackupPath = backupBasePath + "/" + backupName;
    
    // Copy source directory
    if (!copyDirectory(sourceDir, currentBackupPath)) {
        qWarning() << "Backup failed: Cannot copy directory" 
                   << "\nSource:" << sourceDir 
                   << "\nTarget:" << currentBackupPath;
        return false;
    }
    
    // Save metadata (for restoration)
    if (!saveMetadata(sourceDir)) {
        qWarning() << "Backup failed: Cannot save metadata file"
                   << "\nBackup path:" << currentBackupPath
                   << "\nSource directory:" << sourceDir;
        return false;
    }

    qInfo() << "Backup completed successfully:"
            << "\nSource directory:" << sourceDir
            << "\nBackup path:" << currentBackupPath;
    return true;
}

bool BackupManager::createBackupDirectory() {
    QDir dir(backupBasePath);
    if (!dir.exists()) {
        if (!dir.mkpath(backupBasePath)) {
            qWarning() << "Failed to create backup base directory:" << backupBasePath;
            return false;
        }
        qInfo() << "Successfully created backup base directory:" << backupBasePath;
    }
    return true;
}

QString BackupManager::generateBackupName(const QString &sourceDir) {
    QDateTime current = QDateTime::currentDateTime();
    QString timestamp = current.toString("yyyyMMdd_hhmmss");
    QString dirName = QDir(sourceDir).dirName();
    QString backupName = QString("%1_%2").arg(dirName, timestamp);
    qInfo() << "Generated backup name:" << backupName;
    return backupName;
}

bool BackupManager::saveMetadata(const QString &sourceDir) {
    QString metaPath = currentBackupPath + "/metadata.json";
    QFile metaFile(metaPath);
    if (!metaFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot create metadata file:" << metaPath
                   << "\nError:" << metaFile.errorString();
        return false;
    }
    
    QJsonObject metadata;
    metadata["sourcePath"] = sourceDir;
    metadata["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(metadata);
    QByteArray jsonData = doc.toJson();
    
    if (metaFile.write(jsonData) != jsonData.size()) {
        qWarning() << "Failed to write metadata file:" << metaPath
                   << "\nError:" << metaFile.errorString();
        metaFile.close();
        return false;
    }
    
    metaFile.close();
    qInfo() << "Successfully saved metadata file:" << metaPath;
    return true;
}

bool BackupManager::copyDirectory(const QString &sourceDir, const QString &targetPath) {
    QDir dir(sourceDir);
    if (!dir.exists()) {
        return false;
    }

    QDir().mkpath(targetPath);
    
    foreach(QString file, dir.entryList(QDir::Files)) {
        QString srcFile = sourceDir + "/" + file;
        QString destFile = targetPath + "/" + file;
        if (!QFile::copy(srcFile, destFile)) {
            return false;
        }
    }
    return true;
}