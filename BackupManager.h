// BackupManager.h
#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QString>

class BackupManager {
public:
    BackupManager();
    ~BackupManager();

    void backupDirectory(const QString &sourceDir, const QString &targetDir);
    //作为输入递归遍历目录文件树，利用private函数进行备份

private:
    void copyFile(const QString &source, const QString &destination);
    //对于单一文件进行拷贝
};

#endif // BACKUPMANAGER_H