#include "RestoreManager.h"

RestoreManager::RestoreManager(const QString &backupPath, const QString &restorePath)
    : backupPath(backupPath), restorePath(restorePath) {}

bool RestoreManager::restore() {
    QDir backupDir(backupPath);
    if (!backupDir.exists()) {
        qWarning() << "Restore failed: Backup directory does not exist:" << backupPath;
        return false;
    }

    // Read metadata file
    QString originalPath;
    if (!readMetadata(originalPath)) {
        qWarning() << "Restore failed: Cannot read metadata file"
                   << "\nBackup path:" << backupPath;
        return false;
    }

    // If no restore path specified, use original path
    QString targetPath = restorePath.isEmpty() ? originalPath : restorePath;
    qInfo() << "Starting restore:"
            << "\nBackup path:" << backupPath
            << "\nTarget path:" << targetPath;
    
    // Copy backup files
    if (!copyDirectory(backupPath, targetPath)) {
        qWarning() << "Restore failed: Cannot copy files"
                   << "\nSource:" << backupPath
                   << "\nTarget:" << targetPath;
        return false;
    }

    qInfo() << "Restore completed successfully:"
            << "\nTarget path:" << targetPath;
    return true;
}

bool RestoreManager::readMetadata(QString &originalPath) {
    QString metaPath = backupPath + "/metadata.json";
    QFile metaFile(metaPath);
    if (!metaFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to read metadata: Cannot open file"
                   << "\nFile path:" << metaPath
                   << "\nError:" << metaFile.errorString();
        return false;
    }

    QByteArray data = metaFile.readAll();
    metaFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Failed to read metadata: Invalid JSON format"
                   << "\nFile path:" << metaPath;
        return false;
    }

    QJsonObject metadata = doc.object();
    originalPath = metadata["sourcePath"].toString();
    if (originalPath.isEmpty()) {
        qWarning() << "Failed to read metadata: Source path not found"
                   << "\nFile path:" << metaPath;
        return false;
    }

    qInfo() << "Successfully read metadata:"
            << "\nOriginal path:" << originalPath;
    return true;
}

bool RestoreManager::copyDirectory(const QString &sourceDir, const QString &targetDir) {
    QDir dir(sourceDir);
    if (!dir.exists()) {
        return false;
    }

    QDir().mkpath(targetDir);
    
    foreach(QString file, dir.entryList(QDir::Files)) {
        QString srcFile = sourceDir + "/" + file;
        QString destFile = targetDir + "/" + file;
        if (!QFile::copy(srcFile, destFile)) {
            return false;
        }
    }
    return true;
}