#include "RestoreManager.h"
#include "ZipUtils.h"

RestoreManager::RestoreManager(const QString &backupPath, const QString &restorePath)
    : backupPath(backupPath), restorePath(restorePath) {}

bool RestoreManager::restore() {
    QDir backupDir(backupPath);
    if (!backupDir.exists()) {
        qWarning() << "备份目录不存在:" << backupPath;
        return false;
    }

    // 读取元数据文件
    QString originalPath;
    if (!readMetadata(originalPath)) {
        qWarning() << "无法读取元数据文件";
        return false;
    }

    // 如果没有指定恢复路径，使用原始路径
    QString targetPath = restorePath.isEmpty() ? originalPath : restorePath;
    
    // 解压缩备份文件
    QString zipPath = backupPath + "/backup.zip";
    if (!extractZipFile(zipPath, targetPath)) {
        qWarning() << "解压缩失败";
        return false;
    }

    qDebug() << "成功恢复到:" << targetPath;
    return true;
}

bool RestoreManager::readMetadata(QString &originalPath) {
    QFile metaFile(backupPath + "/metadata.json");
    if (!metaFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(metaFile.readAll());
    metaFile.close();

    if (doc.isObject()) {
        QJsonObject metadata = doc.object();
        originalPath = metadata["sourcePath"].toString();
        return !originalPath.isEmpty();
    }
    return false;
}

bool RestoreManager::extractZipFile(const QString &zipPath, const QString &targetDir) {
    return ZipUtils::extractZip(zipPath, targetDir);
}