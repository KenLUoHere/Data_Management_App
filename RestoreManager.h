#ifndef RESTOREMANAGER_H
#define RESTOREMANAGER_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <private/qzipreader_p.h>


class RestoreManager {
public:
    RestoreManager(const QString &backupPath, const QString &restorePath = QString());
    bool restore();

private:
    QString backupPath;   // 备份目录的路径
    QString restorePath;  // 恢复的目标路径
    
    bool extractZipFile(const QString &zipPath, const QString &targetDir);
    bool readMetadata(QString &originalPath);
};

#endif // RESTOREMANAGER_H