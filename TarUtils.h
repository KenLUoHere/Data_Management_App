#ifndef TARUTILS_H
#define TARUTILS_H

#include <QString>
#include <QProcess>
#include <QFileInfo>
#include <QDebug>

class TarUtils {
public:
    // 防止实例化
    TarUtils() = delete;
    
    // 打包目录到tar文件
    static bool compressDirectory(const QString &sourceDir, const QString &tarPath);
    
    // 从tar文件解压到目录
    static bool extractTar(const QString &tarPath, const QString &targetDir);
    
    // 获取tar文件中的目录名
    static QString getTarDirectoryName(const QString &tarPath);

private:
    // 检查目录是否存在
    static bool checkDirectory(const QString &path, bool create = false);
};

#endif // TARUTILS_H 