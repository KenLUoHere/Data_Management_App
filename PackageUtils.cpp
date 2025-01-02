#include "PackageUtils.h"
#include <QDir>
#include <QDebug>

bool PackageUtils::packFiles(const QStringList &files, const QString &packagePath, 
                           const QString &sourceDir) {
    QFile packageFile(packagePath);
    if (!packageFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot create package file:" << packagePath;
        return false;
    }
    
    QDataStream out(&packageFile);
    out.setVersion(QDataStream::Qt_5_15);
    out.setByteOrder(QDataStream::BigEndian);  // 使用大端序
    
    // 写入包头
    PackageHeader header;
    header.magicNumber = MAGIC_NUMBER;
    header.version = VERSION;
    header.fileCount = files.size();
    
    qDebug() << "Writing package header:";
    qDebug() << "- Magic number:" << QString::number(header.magicNumber, 16);
    qDebug() << "- Version:" << header.version;
    qDebug() << "- File count:" << header.fileCount;
    
    // 写入包头
    out << header.magicNumber << header.version << header.fileCount;
    
    // 计算文件头部分的起始偏移量
    qint64 currentOffset = sizeof(qint32) * 3;  // 包头大小
    
    // 写入所有文件头
    for (const QString &filePath : files) {
        if (!writeFileHeader(out, filePath, currentOffset, sourceDir)) {
            packageFile.close();
            return false;
        }
    }
    
    // 写入所有文件内容
    for (const QString &filePath : files) {
        QFileInfo fileInfo(filePath);
        if (fileInfo.isFile()) {
            if (!writeFileContent(out, filePath)) {
                packageFile.close();
                return false;
            }
        }
    }
    
    packageFile.close();
    return true;
}

bool PackageUtils::writeFileHeader(QDataStream &out, const QString &filePath, 
                                 qint64 &offset, const QString &sourceDir) {
    QFileInfo fileInfo(filePath);
    
    // 计算相对路径
    QDir dir(sourceDir);
    QString relativePath = dir.relativeFilePath(filePath);
    
    // 使用 QDataStream 直接写入各个字段
    out << relativePath;  // 写入文件名
    out << (qint64)(fileInfo.isFile() ? fileInfo.size() : 0);  // 写入文件大小
    out << offset;  // 写入偏移量
    out << fileInfo.isDir();  // 写入是否是目录
    
    // 调试输出
    qDebug() << "\nWriting file header:";
    qDebug() << "- Path:" << relativePath;
    qDebug() << "- Size:" << (fileInfo.isFile() ? fileInfo.size() : 0);
    qDebug() << "- Offset:" << offset;
    qDebug() << "- Is Directory:" << fileInfo.isDir();
    
    // 更新偏移量
    if (fileInfo.isFile()) {
        offset += fileInfo.size();
    }
    
    return true;
}

bool PackageUtils::writeFileContent(QDataStream &out, const QString &filePath) {
    QFileInfo fileInfo(filePath);
    
    // 如果是目录，直接返回成功
    if (fileInfo.isDir()) {
        return true;
    }
    
    // 如果是文件，则读取内容
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
    // 确保目标目录存在并可写
    QDir dir(targetDir);
    if (!dir.exists()) {
        if (!dir.mkpath(targetDir)) {
            qWarning() << "Cannot create target directory:" << targetDir;
            return false;
        }
    }

    // 检查目标目录权限
    QFileInfo targetInfo(targetDir);
    if (!targetInfo.isWritable()) {
        qWarning() << "Target directory is not writable:" << targetDir;
        return false;
    }

    QFile packageFile(packagePath);
    if (!packageFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open package file:" << packagePath;
        return false;
    }
    
    QDataStream in(&packageFile);
    in.setVersion(QDataStream::Qt_5_15);
    in.setByteOrder(QDataStream::BigEndian);  // 使用大端序
    
    // 读取包头
    PackageHeader header;
    in >> header.magicNumber >> header.version >> header.fileCount;
    
    qDebug() << "Read package header:";
    qDebug() << "- Magic number:" << QString::number(header.magicNumber, 16);
    qDebug() << "- Version:" << header.version;
    qDebug() << "- File count:" << header.fileCount;
    
    // 读取所有文件头并解压文件
    QList<FileHeader> fileHeaders;  // 先收集所有文件头
    for (int i = 0; i < header.fileCount; ++i) {
        FileHeader fileHeader;
        if (!readFileHeader(in, fileHeader)) {
            qWarning() << "Failed to read file header at index" << i;
            packageFile.close();
            return false;
        }
        fileHeaders.append(fileHeader);
    }
    
    // 按照偏移量顺序处理文件
    for (const FileHeader &fileHeader : fileHeaders) {
        if (!extractFile(in, fileHeader, targetDir)) {
            qWarning() << "Failed to extract file:" << fileHeader.filename;
            packageFile.close();
            return false;
        }
    }
    
    packageFile.close();
    return true;
}

bool PackageUtils::readFileHeader(QDataStream &in, FileHeader &header) {
    // 清空结构体
    memset(&header, 0, sizeof(FileHeader));
    
    // 读取各个字段
    QString relativePath;
    qint64 fileSize;
    qint64 fileOffset;
    bool isDir;
    
    in >> relativePath;  // 读取文件名
    in >> fileSize;      // 读取文件大小
    in >> fileOffset;    // 读取偏移量
    in >> isDir;         // 读取是否是目录
    
    // 验证数据的合理性
    if (relativePath.isEmpty() || fileSize < 0 || fileOffset < 0) {
        qWarning() << "Invalid header values:"
                   << "\nPath:" << relativePath
                   << "\nSize:" << fileSize
                   << "\nOffset:" << fileOffset;
        return false;
    }
    
    // 复制文件名到结构体
    QByteArray pathData = relativePath.toUtf8();
    int copySize = qMin(pathData.size(), (int)sizeof(header.filename) - 1);
    memcpy(header.filename, pathData.constData(), copySize);
    header.filename[copySize] = '\0';  // 确保字符串结束
    
    // 设置其他字段
    header.fileSize = fileSize;
    header.offset = fileOffset;
    header.isDirectory = isDir;
    
    // 调试输出
    qDebug() << "\nRead file header:";
    qDebug() << "- Path:" << QString::fromUtf8(header.filename);
    qDebug() << "- Size:" << header.fileSize;
    qDebug() << "- Offset:" << header.offset;
    qDebug() << "- Is Directory:" << header.isDirectory;
    
    return true;
}

bool PackageUtils::extractFile(QDataStream &in, const FileHeader &header, const QString &targetDir) {
    // 构建完整的目标文件路径
    QString fileName = QString::fromUtf8(header.filename);
    QString filePath = QDir(targetDir).filePath(fileName);
    
    qDebug() << "Extracting file:";
    qDebug() << "- Source path:" << fileName;
    qDebug() << "- Target path:" << filePath;
    
    // 如果是目录
    if (header.isDirectory) {
        return QDir().mkpath(filePath);
    }
    
    // 确保父目录存在
    QDir().mkpath(QFileInfo(filePath).absolutePath());
    
    // 创建并写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot create file:" << filePath;
        return false;
    }
    
    // 读取并写入文件内容
    QByteArray buffer;
    qint64 remaining = header.fileSize;
    
    while (remaining > 0) {
        buffer = in.device()->read(qMin(remaining, qint64(8192)));
        if (buffer.isEmpty()) break;
        
        if (file.write(buffer) != buffer.size()) {
            qWarning() << "Failed to write file data:" << file.errorString();
            file.close();
            return false;
        }
        remaining -= buffer.size();
    }
    
    file.close();
    return remaining == 0;
}