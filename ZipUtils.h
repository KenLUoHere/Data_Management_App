#ifndef ZIPUTILS_H
#define ZIPUTILS_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <private/qzipwriter_p.h>
#include <private/qzipreader_p.h>

class ZipUtils {
public:
    static bool compressDirectory(const QString &sourceDir, const QString &zipPath);
    static bool extractZip(const QString &zipPath, const QString &targetDir);

private:
    // 私有构造函数，防止实例化
    ZipUtils() {}
};

#endif // ZIPUTILS_H 