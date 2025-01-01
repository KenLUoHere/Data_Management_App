#include "PackageUtils.h"
#include <QDir>
#include <QDebug>

bool PackageUtils::packFiles(const QStringList &files, const QString &packagePath) {
    QFile packageFile(packagePath);
    if (!packageFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot create package file:" << packagePath;
        return false;
    }
    
    QDataStream out(&packageFile);
    out.setVersion(QDataStream::Qt_5_15);
    
    // 写入包头
    PackageHeader header;
    header.magicNumber = MAGIC_NUMBER;
    header.version = VERSION;
    header.fileCount = files.size();
    
    out << header.magicNumber << header.version << header.fileCount;
    
    // 计算文件头部分的大小
    qint64 headerOffset = sizeof(PackageHeader);
    qint64 currentOffset = headerOffset + files.size() * sizeof(FileHeader);
    
    // 写入所有文件头
    for (const QString &filePath : files) {
        if (!writeFileHeader(out, filePath, currentOffset)) {
            packageFile.close();
            return false;
        }
    }
    
    // 写入所有文件内容
    for (const QString &filePath : files) {
        if (!writeFileContent(out, filePath)) {
            packageFile.close();
            return false;
        }
    }
    
    packageFile.close();
    return true;
}

bool PackageUtils::writeFileHeader(QDataStream &out, const QString &filePath, qint64 &offset) {
    QFileInfo fileInfo(filePath);
    FileHeader header;
    
    // 填充文件头信息
    strncpy(header.filename, fileInfo.fileName().toUtf8().constData(), 255);
    header.filename[255] = '\0';
    header.fileSize = fileInfo.size();
    header.offset = offset;
    header.isDirectory = fileInfo.isDir();
    
    // 写入文件头
    out.writeRawData((char*)&header, sizeof(FileHeader));
    
    // 更新下一个文件的偏移量
    offset += header.fileSize;
    return true;
}

bool PackageUtils::writeFileContent(QDataStream &out, const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file for reading:" << filePath;
        return false;
    }
    
    // 写入文件内容
    QByteArray buffer;
    while (!file.atEnd()) {
        buffer = file.read(8192);  // 8KB chunks
        out.writeRawData(buffer.constData(), buffer.size());
    }
    
    file.close();
    return true;
}

bool PackageUtils::unpackFiles(const QString &packagePath, const QString &targetDir) {
    QFile packageFile(packagePath);
    if (!packageFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open package file:" << packagePath;
        return false;
    }
    
    QDataStream in(&packageFile);
    in.setVersion(QDataStream::Qt_5_15);
    
    // 读取包头
    PackageHeader header;
    in >> header.magicNumber >> header.version >> header.fileCount;
    
    if (header.magicNumber != MAGIC_NUMBER) {
        qWarning() << "Invalid package file format";
        packageFile.close();
        return false;
    }
    
    // 创建目标目录
    QDir dir;
    if (!dir.mkpath(targetDir)) {
        qWarning() << "Cannot create target directory:" << targetDir;
        packageFile.close();
        return false;
    }
    
    // 读取所有文件头并解压文件
    for (int i = 0; i < header.fileCount; ++i) {
        FileHeader fileHeader;
        if (!readFileHeader(in, fileHeader)) {
            packageFile.close();
            return false;
        }
        
        if (!extractFile(in, fileHeader, targetDir)) {
            packageFile.close();
            return false;
        }
    }
    
    packageFile.close();
    return true;
}

bool PackageUtils::readFileHeader(QDataStream &in, FileHeader &header) {
    return in.readRawData((char*)&header, sizeof(FileHeader)) == sizeof(FileHeader);
}

bool PackageUtils::extractFile(QDataStream &in, const FileHeader &header, const QString &targetDir) {
    QString filePath = targetDir + "/" + QString::fromUtf8(header.filename);
    
    if (header.isDirectory) {
        QDir dir;
        return dir.mkpath(filePath);
    }
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot create file:" << filePath;
        return false;
    }
    
    // 读取并写入文件内容
    char buffer[8192];  // 8KB buffer
    qint64 remaining = header.fileSize;
    
    while (remaining > 0) {
        qint64 toRead = qMin(remaining, (qint64)sizeof(buffer));
        qint64 bytesRead = in.readRawData(buffer, toRead);
        if (bytesRead <= 0) break;
        
        file.write(buffer, bytesRead);
        remaining -= bytesRead;
    }
    
    file.close();
    return remaining == 0;
}