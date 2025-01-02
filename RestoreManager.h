#ifndef RESTOREMANAGER_H
#define RESTOREMANAGER_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "PackageUtils.h"

class RestoreManager {
public:
    RestoreManager(const QString &backupPath, const QString &restorePath)
        : backupPath(backupPath), targetPath(restorePath) {}
    
    bool restore();

private:
    bool readMetadata(QString* originalPath);
    QString backupPath;
    QString targetPath;
};

#endif // RESTOREMANAGER_H