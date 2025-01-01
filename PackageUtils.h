#ifndef PACKAGEUTILS_H
#define PACKAGEUTILS_H

#include <QString>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>

// 文件头结构
struct FileHeader {
    char filename[256];     // 文件名
    qint64 fileSize;       // 文件大小
    qint64 offset;         // 文件在包中的偏移量
    bool isDirectory;      // 是否是目录
};

class PackageUtils {
public:
    static bool packFiles(const QStringList &files, const QString &packagePath);
    static bool unpackFiles(const QString &packagePath, const QString &targetDir);
    
private:
    static const qint32 MAGIC_NUMBER = 0x504B4344;  // "PKCD"
    static const qint32 VERSION = 1;
    
    // 包文件头
    struct PackageHeader {
        qint32 magicNumber;    // 魔数，用于识别文件格式
        qint32 version;        // 版本号
        qint32 fileCount;      // 文件数量
    };
    
    static bool writeFileHeader(QDataStream &out, const QString &filePath, qint64 &offset);
    static bool writeFileContent(QDataStream &out, const QString &filePath);
    static bool readFileHeader(QDataStream &in, FileHeader &header);
    static bool extractFile(QDataStream &in, const FileHeader &header, const QString &targetDir);
};

#endif // PACKAGEUTILS_H