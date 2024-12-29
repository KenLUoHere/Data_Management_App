#include "RestoreManager.h"
#include "ZipUtils.h"

RestoreManager::RestoreManager(const QString &backupPath, const QString &restorePath)
    : backupPath(backupPath), restorePath(restorePath) {}

bool RestoreManager::restore() {
    QDir backupDir(backupPath);
    if (!backupDir.exists()) {
        qWarning() << "恢复失败：备份目录不存在:" << backupPath;
        return false;
    }

    // 读取元数据文件
    QString originalPath;
    if (!readMetadata(originalPath)) {
        qWarning() << "恢复失败：无法读取元数据文件"
                   << "\n备份路径:" << backupPath;
        return false;
    }

    // 如果没有指定恢复路径，使用原始路径
    QString targetPath = restorePath.isEmpty() ? originalPath : restorePath;
    qInfo() << "开始恢复备份:"
            << "\n备份路径:" << backupPath
            << "\n目标路径:" << targetPath;
    
    // 解压缩备份文件
    QString zipPath = backupPath + "/backup.zip";
    if (!extractZipFile(zipPath, targetPath)) {
        qWarning() << "恢复失败：解压缩失败"
                   << "\nZIP文件:" << zipPath
                   << "\n目标路径:" << targetPath;
        return false;
    }

    qInfo() << "恢复成功完成:"
            << "\n目标路径:" << targetPath;
    return true;
}

bool RestoreManager::readMetadata(QString &originalPath) {
    QString metaPath = backupPath + "/metadata.json";
    QFile metaFile(metaPath);
    if (!metaFile.open(QIODevice::ReadOnly)) {
        qWarning() << "读取元数据失败：无法打开文件"
                   << "\n文件路径:" << metaPath
                   << "\n错误:" << metaFile.errorString();
        return false;
    }

    QByteArray data = metaFile.readAll();
    metaFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "读取元数据失败：JSON格式无效"
                   << "\n文件路径:" << metaPath;
        return false;
    }

    QJsonObject metadata = doc.object();
    originalPath = metadata["sourcePath"].toString();
    if (originalPath.isEmpty()) {
        qWarning() << "读取元数据失败：未找到源路径信息"
                   << "\n文件路径:" << metaPath;
        return false;
    }

    qInfo() << "成功读取元数据:"
            << "\n原始路径:" << originalPath;
    return true;
}

bool RestoreManager::extractZipFile(const QString &zipPath, const QString &targetDir) {
    qInfo() << "开始解压文件:"
            << "\nZIP文件:" << zipPath
            << "\n目标目录:" << targetDir;
    
    bool success = ZipUtils::extractZip(zipPath, targetDir);
    if (!success) {
        qWarning() << "调用ZipUtils解压失败";
    }
    return success;
}