# 备份还原管理器

## 项目简介

这是一个基于 Qt6 开发的备份还原管理器，提供了简单直观的图形界面，支持文件和目录的备份与还原功能。

## 功能特点

### 备份功能

- 支持选择源目录进行备份
- 自动记录源目录路径
- 生成元数据文件，保存备份信息
- 文件打包压缩

### 还原功能

- 支持选择备份文件进行还原
- 可以还原到原始位置
- 支持自定义还原路径
- 自动检查目标位置的写入权限

## 技术实现

- 开发环境：Qt 6.8.1
- 编程语言：C++17
- 主要模块：
  - BackupManager：负责文件备份和打包
  - RestoreManager：负责文件还原和解包
  - PackageUtils：提供文件打包和解包功能

## 使用说明

### 主界面

"I Want to Back Up"：进入备份界面
"I Want to Restore"：进入还原界面

### 备份操作

选择需要备份的源目录
选择备份文件保存位置
点击确认开始备份

### 还原操作

选择备份文件
可选：指定还原位置（默认还原到原始位置）
点击确认开始还原

## 项目结构

### 编译说明

1. 确保已安装 Qt 6.8.1 或更高版本
2. 使用 Qt Creator 打开项目
3. 执行以下命令：

```bash
qmake project_SDE.pro
make clean
make
```

## 注意事项

- 确保有足够的磁盘空间进行备份
- 还原时请确保目标位置有写入权限
- 建议定期测试备份文件的完整性

## 开发团队

Ken Luo

## 版本历史

- v0.1: 初始版本
  - 实现基本的备份还原功能
  - 添加图形界面
  - 支持元数据保存

## 贡献

-这是 Ken Luo 的个人项目，主要负责备份和还原功能的实现。
