#ifndef RESTOREMANAGER_H
#define RESTOREMANAGER_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QDebug>

class RestoreManager {
public:
    RestoreManager(const QString &backupPath, const QString &restorePath);
    bool restore(); // 执行恢复操作

private:
    QString backupPath;   // 备份文件或目录的路径
    QString restorePath;  // 恢复的目标路径

    bool restoreFile(const QString &sourcePath, const QString &targetPath);  // 恢复单个文件
    bool restoreDirectory(const QString &sourcePath, const QString &targetPath); // 恢复目录
};

#endif // RESTOREMANAGER_H