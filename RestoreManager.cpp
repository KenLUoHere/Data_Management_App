#include "RestoreManager.h"

RestoreManager::RestoreManager(const QString &backupPath, const QString &restorePath)
    : backupPath(backupPath), restorePath(restorePath) {}

// 恢复操作的主函数
bool RestoreManager::restore() {
    QFileInfo backupInfo(backupPath);

    if (!backupInfo.exists()) {
        qWarning() << "Backup path does not exist:" << backupPath;
        return false;
    }

    if (backupInfo.isDir()) {
        // 如果备份路径是一个目录，递归恢复整个目录
        return restoreDirectory(backupPath, restorePath);
    } else if (backupInfo.isFile()) {
        // 如果备份路径是一个文件，直接恢复
        QString targetFilePath = restorePath + "/" + backupInfo.fileName();
        return restoreFile(backupPath, targetFilePath);
    } else {
        qWarning() << "Unsupported backup path type:" << backupPath;
        return false;
    }
}

// 恢复单个文件
bool RestoreManager::restoreFile(const QString &sourcePath, const QString &targetPath) {
    QFile sourceFile(sourcePath);

    if (!sourceFile.exists()) {
        qWarning() << "Source file does not exist:" << sourcePath;
        return false;
    }

    // 确保目标目录存在
    QDir targetDir = QFileInfo(targetPath).absoluteDir();
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(targetDir.absolutePath())) {
            qWarning() << "Failed to create target directory:" << targetDir.absolutePath();
            return false;
        }
    }

    // 执行文件拷贝
    if (!sourceFile.copy(targetPath)) {
        qWarning() << "Failed to restore file:" << sourcePath << "to" << targetPath;
        return false;
    }

    qDebug() << "Restored file:" << sourcePath << "to" << targetPath;
    return true;
}

// 恢复目录
bool RestoreManager::restoreDirectory(const QString &sourcePath, const QString &targetPath) {
    QDir sourceDir(sourcePath), targetDir(targetPath);

    if (!sourceDir.exists()) {
        qWarning() << "Source directory does not exist:" << sourcePath;
        return false;
    }

    // 确保目标目录存在
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(targetPath)) {
            qWarning() << "Failed to create target directory:" << targetPath;
            return false;
        }
    }

    // 遍历源目录的所有条目
    for (const QFileInfo &entry : sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString sourceEntryPath = entry.filePath();
        QString targetEntryPath = targetPath + "/" + entry.fileName();

        if (entry.isDir()) {
            // 如果是目录，递归恢复
            if (!restoreDirectory(sourceEntryPath, targetEntryPath)) {
                return false;
            }
        } else if (entry.isFile()) {
            // 如果是文件，恢复文件
            if (!restoreFile(sourceEntryPath, targetEntryPath)) {
                return false;
            }
        }
    }

    qDebug() << "Restored directory:" << sourcePath << "to" << targetPath;
    return true;
}