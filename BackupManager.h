#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include "PackageUtils.h"

class BackupManager {
public:
    BackupManager(const QString &backupBasePath = "backups");
    ~BackupManager();

    bool backup(const QString &sourceDir);
    QString getCurrentBackupPath() const { return currentBackupPath; }
    
private:
    QString backupBasePath;
    QString currentBackupPath;
    
    bool packDirectory(const QString &sourceDir, const QString &packagePath);
    bool saveMetadata(const QString &sourceDir);
    QString generateBackupName(const QString &sourceDir);
};

#endif // BACKUPMANAGER_H