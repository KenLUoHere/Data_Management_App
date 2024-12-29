#ifndef ZIPUTILS_H
#define ZIPUTILS_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDirIterator>
#include <private/qzipwriter_p.h>
#include <private/qzipreader_p.h>

class ZipUtils {
public:
    // 2GB 大小限制
    static constexpr qint64 MAX_FILE_SIZE = 2LL * 1024 * 1024 * 1024;
    
    static bool compressDirectory(const QString &sourceDir, const QString &zipPath);
    static bool extractZip(const QString &zipPath, const QString &targetDir);

private:
    // 私有构造函数，防止实例化
    ZipUtils() {}
};

#endif // ZIPUTILS_H 