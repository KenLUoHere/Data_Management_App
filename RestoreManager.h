#ifndef RESTOREMANAGER_H
#define RESTOREMANAGER_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class RestoreManager {
public:
    RestoreManager(const QString &backupPath, const QString &restorePath = QString());
    bool restore();

private:
    QString backupPath;   // Backup directory path
    QString restorePath;  // Restore target path
    
    bool copyDirectory(const QString &sourceDir, const QString &targetDir);
    bool readMetadata(QString &originalPath);
};

#endif // RESTOREMANAGER_H