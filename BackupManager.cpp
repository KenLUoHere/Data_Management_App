#include "BackupManager.h"
#include "ZipUtils.h"

BackupManager::BackupManager(const QString &backupBasePath)
    : backupBasePath(backupBasePath) {}

bool BackupManager::backup(const QString &sourceDir) {
    // 创建备份目录
    if (!createBackupDirectory()) {
        qWarning() << "备份失败：无法创建备份目录" << backupBasePath;
        return false;
    }
    
    // 生成备份名称
    QString backupName = generateBackupName(sourceDir);
    currentBackupPath = backupBasePath + "/" + backupName;
    
    // 压缩源目录
    QString zipPath = currentBackupPath + "/backup.zip";
    if (!compressDirectory(sourceDir, zipPath)) {
        qWarning() << "备份失败：压缩目录失败" 
                   << "\n源目录:" << sourceDir 
                   << "\n目标ZIP:" << zipPath;
        return false;
    }
    
    // 保存元数据（用于恢复）
    if (!saveMetadata(sourceDir)) {
        qWarning() << "备份失败：无法保存元数据文件"
                   << "\n备份路径:" << currentBackupPath
                   << "\n源目录:" << sourceDir;
        return false;
    }

    qInfo() << "备份成功完成:"
            << "\n源目录:" << sourceDir
            << "\n备份路径:" << currentBackupPath;
    return true;
}

// 创建备份目录
bool BackupManager::createBackupDirectory() {
    QDir dir(backupBasePath);
    if (!dir.exists()) {
        if (!dir.mkpath(backupBasePath)) {
            qWarning() << "创建备份基础目录失败:" << backupBasePath;
            return false;
        }
        qInfo() << "成功创建备份基础目录:" << backupBasePath;
    }
    return true;
}

QString BackupManager::generateBackupName(const QString &sourceDir) {
    QDateTime current = QDateTime::currentDateTime();
    QString timestamp = current.toString("yyyyMMdd_hhmmss");
    QString dirName = QDir(sourceDir).dirName();
    QString backupName = QString("%1_%2").arg(dirName, timestamp);
    qInfo() << "生成备份名称:" << backupName;
    return backupName;
}

bool BackupManager::compressDirectory(const QString &sourceDir, const QString &zipPath) {
    qInfo() << "开始压缩目录:"
            << "\n源目录:" << sourceDir
            << "\n目标ZIP:" << zipPath;
    bool success = ZipUtils::compressDirectory(sourceDir, zipPath);
    if (!success) {
        qWarning() << "调用ZipUtils压缩目录失败";
    }
    return success;
}

bool BackupManager::saveMetadata(const QString &sourceDir) {
    QString metaPath = currentBackupPath + "/metadata.json";
    QFile metaFile(metaPath);
    if (!metaFile.open(QIODevice::WriteOnly)) {
        qWarning() << "无法创建元数据文件:" << metaPath
                   << "\n错误:" << metaFile.errorString();
        return false;
    }
    
    QJsonObject metadata;
    metadata["sourcePath"] = sourceDir;
    metadata["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    QJsonDocument doc(metadata);
    QByteArray jsonData = doc.toJson();
    
    if (metaFile.write(jsonData) != jsonData.size()) {
        qWarning() << "写入元数据文件失败:" << metaPath
                   << "\n错误:" << metaFile.errorString();
        metaFile.close();
        return false;
    }
    
    metaFile.close();
    qInfo() << "成功保存元数据文件:" << metaPath;
    return true;
}