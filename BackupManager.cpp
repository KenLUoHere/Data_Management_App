#include "BackupManager.h"
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>

BackupManager::BackupManager(const QString &backupBasePath)
    : backupBasePath(backupBasePath) {
}

BackupManager::~BackupManager() {
}

bool BackupManager::backup(const QString &sourceDir) {
    // 检查源目录是否存在且可访问
    QDir dir(sourceDir);
    if (!dir.exists()) {
        qWarning() << "Source directory does not exist:" << sourceDir;
        return false;
    }
    
    // 检查备份基础目录是否可写
    QFileInfo backupDirInfo(backupBasePath);
    if (!backupDirInfo.isWritable()) {
        qWarning() << "Backup directory is not writable:" << backupBasePath;
        return false;
    }
    
    // 检查备份基础目录权限
    if (!QDir().mkpath(backupBasePath)) {
        qWarning() << "Cannot create backup base directory:" << backupBasePath;
        return false;
    }
    
    // 尝试创建一个测试文件来验证写入权限
    QString testFile = backupBasePath + "/.test_write";
    QFile file(testFile);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "No write permission in backup directory:" << backupBasePath;
        return false;
    }
    file.close();
    file.remove();
    
    // 生成备份名称和创建备份目录
    QString backupName = generateBackupName(sourceDir);
    currentBackupPath = backupBasePath + "/" + backupName;
    
    if (!QDir().mkpath(currentBackupPath)) {
        qWarning() << "Cannot create backup directory:" << currentBackupPath;
        return false;
    }
    
    // 打包目录
    QString packagePath = currentBackupPath + "/backup.pkg";
    if (!packDirectory(sourceDir, packagePath)) {
        qWarning() << "Backup failed: Cannot pack directory"
                   << "\nSource:" << sourceDir
                   << "\nPackage:" << packagePath;
        return false;
    }
    
    // 保存元数据
    if (!saveMetadata(sourceDir)) {
        qWarning() << "Backup failed: Cannot save metadata file";
        return false;
    }
    
    qInfo() << "Backup completed successfully:"
            << "\nSource directory:" << sourceDir
            << "\nBackup path:" << currentBackupPath;
    return true;
}

bool BackupManager::packDirectory(const QString &sourceDir, const QString &packagePath) {
    QStringList files;
    QDir sourceDirectory(sourceDir);
    QString sourcePath = sourceDirectory.absolutePath();
    
    // 收集所有文件和目录
    QDirIterator it(sourcePath, 
                    QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);
    
    while (it.hasNext()) {
        QString path = it.next();
        files << path;
    }
    
    // 调用时需要传递三个参数
    return PackageUtils::packFiles(files, packagePath, sourcePath);  // 添加 sourcePath 参数
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
    metadata["originalPath"] = sourceDir;
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

//bool BackupManager::copyDirectory(const QString &sourceDir, const QString &targetPath) {
//    QDir dir(sourceDir);
//    if (!dir.exists()) {
//        return false;
//    }

//    QDir().mkpath(targetPath);
    
//    foreach(QString file, dir.entryList(QDir::Files)) {
//        QString srcFile = sourceDir + "/" + file;
//        QString destFile = targetPath + "/" + file;
//        if (!QFile::copy(srcFile, destFile)) {
//            return false;
//        }
//    }
//    return true;
//}