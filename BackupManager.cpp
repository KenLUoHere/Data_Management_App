// BackupManager.cpp
#include "BackupManager.h"
#include <QFile>
#include <QDir>
#include <QDebug>

BackupManager::BackupManager() {}

BackupManager::~BackupManager() {}

void BackupManager::backupDirectory(const QString &sourceDir, const QString &targetDir) {
    QDir source(sourceDir); //源路径
    if (!source.exists()) {
        qWarning() << "Source directory does not exist:" << sourceDir;
        return;
    }
    //判断目录是否存在

    QDir target(targetDir); //目标路径
    if (!target.exists()) {
        target.mkpath(targetDir);
    }
    //不存在则建立路径
    for (const QFileInfo &entry : source.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString sourcePath = entry.filePath();
        QString targetPath = targetDir + "/" + entry.fileName();

        if (entry.isDir()) {
            backupDirectory(sourcePath, targetPath); // 递归处理子目录
        }
        else if (entry.isFile()) {
            copyFile(sourcePath, targetPath); //复制文件
        }
        else {
            qWarning() << "Unsupported file type:" << sourcePath;
        }
    }
}

void BackupManager::copyFile(const QString &source, const QString &destination) {
    if (QFile::copy(source, destination)) {
        qDebug() << "File copied:" << source << "->" << destination;
    } else {
        qWarning() << "Failed to copy file:" << source;
    }
}
