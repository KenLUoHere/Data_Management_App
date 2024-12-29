#include "ZipUtils.h"

bool ZipUtils::compressDirectory(const QString &sourceDir, const QString &zipPath) {
    QZipWriter zip(zipPath);
    QDir dir(sourceDir);
    
    QDirIterator it(sourceDir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, 
                    QDirIterator::Subdirectories);
    
    while (it.hasNext()) {
        QString filePath = it.next();
        QString relativePath = dir.relativeFilePath(filePath);
        
        if (QFileInfo(filePath).isFile()) {
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly)) {
                zip.addFile(relativePath, file.readAll());
                file.close();
            }
        } else if (QFileInfo(filePath).isDir()) {
            zip.addDirectory(relativePath);
        }
    }
    
    zip.close();
    return true;
}

bool ZipUtils::extractZip(const QString &zipPath, const QString &targetDir) {
    QZipReader zip(zipPath);
    if (!zip.isReadable()) {
        return false;
    }

    // 创建目标目录
    QDir dir;
    if (!dir.mkpath(targetDir)) {
        return false;
    }

    // 解压所有文件
    const QList<QZipReader::FileInfo> allFiles = zip.fileInfoList();
    for (const QZipReader::FileInfo &info : allFiles) {
        const QString filePath = targetDir + "/" + info.filePath;
        
        if (info.isDir) {
            QDir dir;
            dir.mkpath(filePath);
        } else {
            QDir dir;
            dir.mkpath(QFileInfo(filePath).path());
            
            QFile file(filePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(zip.fileData(info.filePath));
                file.close();
            }
        }
    }

    zip.close();
    return true;
} 