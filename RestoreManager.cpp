#include "RestoreManager.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

bool RestoreManager::restore() {
    qDebug() << "Starting restore process...";
    qDebug() << "Backup path:" << backupPath;
    
    // 首先读取元数据获取原始路径
    QString originalPath;
    if (!readMetadata(&originalPath)) {
        qWarning() << "Restore failed: Cannot read metadata";
        return false;
    }
    
    // 如果没有指定目标路径，使用原始路径
    if (targetPath.isEmpty()) {
        targetPath = originalPath;
        qDebug() << "Using original path as target path:" << targetPath;
    }
    
    // 确保目标目录存在
    QDir targetDir(targetPath);
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(targetPath)) {
            qWarning() << "Cannot create target directory:" << targetPath;
            return false;
        }
    }
    
    // 检查目标目录是否可写
    QFileInfo targetInfo(targetPath);
    if (!targetInfo.isWritable()) {
        qWarning() << "Target directory is not writable:" << targetPath;
        return false;
    }
    
    // 使用 PackageUtils 解包文件
    QString packagePath = backupPath + "/backup.pkg";
    if (!PackageUtils::unpackFiles(packagePath, targetPath)) {
        qWarning() << "Restore failed: Cannot unpack files";
        return false;
    }
    
    qDebug() << "Restore completed successfully to:" << targetPath;
    return true;
}

bool RestoreManager::readMetadata(QString* originalPath) {
    // 构建元数据文件路径
    QString metadataPath = backupPath + "/metadata.json";
    
    // 打开元数据文件
    QFile metadataFile(metadataPath);
    if (!metadataFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open metadata file:" << metadataPath;
        return false;
    }
    
    // 读取元数据
    QByteArray jsonData = metadataFile.readAll();
    metadataFile.close();
    
    // 解析 JSON
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qWarning() << "Invalid metadata JSON format";
        return false;
    }
    
    QJsonObject metadata = doc.object();
    
    // 验证元数据
    if (!metadata.contains("originalPath")) {
        qWarning() << "Metadata missing originalPath";
        return false;
    }
    
    // 获取原始路径
    *originalPath = metadata["originalPath"].toString();
    qInfo() << "Successfully read metadata: \nOriginal path:" << *originalPath;
    
    return true;
}