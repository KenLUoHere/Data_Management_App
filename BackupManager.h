#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>

class BackupManager {
public:
    BackupManager(const QString &backupBasePath = "backups");
    ~BackupManager();

    bool backup(const QString &sourceDir);
    
private:
    QString backupBasePath;
    QString currentBackupPath;
    
    bool createBackupDirectory();
    bool copyDirectory(const QString &sourceDir, const QString &targetPath);
    bool saveMetadata(const QString &sourceDir);
    QString generateBackupName(const QString &sourceDir);
};

#endif // BACKUPMANAGER_H