#include "ZipUtils.h"

bool ZipUtils::compressDirectory(const QString &sourceDir, const QString &zipPath) {
    // 检查源目录是否存在
    if (!QDir(sourceDir).exists()) {
        qWarning() << "源目录不存在:" << sourceDir;
        return false;
    }

    // 检查目标zip路径是否可写
    QFileInfo zipFileInfo(zipPath);
    QDir zipDir = zipFileInfo.dir();
    if (!zipDir.exists() && !zipDir.mkpath(".")) {
        qWarning() << "无法创建zip文件目录:" << zipDir.path();
        return false;
    }

    QZipWriter zip(zipPath);
    if (zip.status() != QZipWriter::NoError) {
        qWarning() << "创建zip文件失败:" << zipPath;
        return false;
    }

    QDir dir(sourceDir);
    QDirIterator it(sourceDir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, 
                    QDirIterator::Subdirectories);

    int fileCount = 0;  // 添加文件计数
    bool hasError = false;

    while (it.hasNext()) {
        QString filePath = it.next();
        QString relativePath = dir.relativeFilePath(filePath);
        
        if (QFileInfo(filePath).isFile()) {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) {
                qWarning() << "无法打开文件:" << filePath << "错误:" << file.errorString();
                continue; // 跳过这个文件，继续处理其他文件
            }

            // 检查文件大小
            if (file.size() > MAX_FILE_SIZE) { // 比如 MAX_FILE_SIZE = 2GB
                qWarning() << "文件过大:" << filePath;
                file.close();
                continue;
            }

            try {
                zip.addFile(relativePath, file.readAll());
                fileCount++;  // 成功添加文件后增加计数
            } catch (const std::exception& e) {
                qWarning() << "添加文件到zip失败:" << filePath << "错误:" << e.what();
                hasError = true;
            }
            file.close();
        } 
        else if (QFileInfo(filePath).isDir()) {
            try {
                zip.addDirectory(relativePath);
                fileCount++;  // 成功添加目录后增加计数
            } catch (const std::exception& e) {
                qWarning() << "添加目录到zip失败:" << filePath << "错误:" << e.what();
                hasError = true;
            }
        }
    }
    
    zip.close();
    if (!hasError) {
        qInfo() << "压缩完成:" << zipPath << "共处理" << fileCount << "个文件/目录";
    } else {
        qWarning() << "压缩过程中出现错误:" << zipPath;
    }
    return !hasError;
}

bool ZipUtils::extractZip(const QString &zipPath, const QString &targetDir) {
    // 检查zip文件是否存在
    if (!QFile::exists(zipPath)) {
        qWarning() << "Zip文件不存在:" << zipPath;
        return false;
    }

    QZipReader zip(zipPath);
    if (!zip.isReadable()) {
        qWarning() << "无法读取zip文件:" << zipPath;
        return false;
    }

    // 检查目标目录
    QDir dir(targetDir);
    if (!dir.exists() && !dir.mkpath(".")) {
        qWarning() << "无法创建目标目录:" << targetDir;
        return false;
    }

    // 检查目标目录是否可写
    QFileInfo targetDirInfo(targetDir);
    if (!targetDirInfo.isWritable()) {
        qWarning() << "目标目录没有写入权限:" << targetDir;
        return false;
    }

    bool hasError = false;
    int extractedCount = 0;  // 添加解压计数
    const QList<QZipReader::FileInfo> allFiles = zip.fileInfoList();
    
    qInfo() << "开始解压:" << zipPath << "到" << targetDir;
    qInfo() << "共有" << allFiles.size() << "个文件/目录待处理";

    for (const QZipReader::FileInfo &info : allFiles) {
        const QString filePath = targetDir + "/" + info.filePath;
        
        if (info.isDir) {
            QDir dir;
            if (!dir.mkpath(filePath)) {
                qWarning() << "创建目录失败:" << filePath;
                hasError = true;
                break;
            }
            extractedCount++;
        } 
        else {
            // 检查文件路径是否包含 ".."，防止目录遍历攻击
            if (info.filePath.contains("..")) {
                qWarning() << "检测到不安全的文件路径:" << info.filePath;
                continue;
            }

            QDir dir;
            if (!dir.mkpath(QFileInfo(filePath).path())) {
                qWarning() << "创建父目录失败:" << QFileInfo(filePath).path();
                continue;
            }
            
            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly)) {
                qWarning() << "无法创建文件:" << filePath << "错误:" << file.errorString();
                continue;
            }

            QByteArray data = zip.fileData(info.filePath);
            if (data.isEmpty()) {
                qWarning() << "读取zip文件数据失败:" << info.filePath;
                file.close();
                continue;
            }

            if (file.write(data) != data.size()) {
                qWarning() << "写入文件数据失败:" << filePath;
                file.close();
                continue;
            }
            file.close();
            extractedCount++;
        }
    }

    zip.close();
    if (!hasError) {
        qInfo() << "解压完成:" << zipPath << "共解压" << extractedCount << "/" << allFiles.size() << "个文件/目录";
    }
    else {
        qWarning() << "解压过程中出现错误:" << zipPath << "已处理" << extractedCount << "/" << allFiles.size() << "个文件/目录";
    }
    return !hasError;
} 