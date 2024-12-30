QT += testlib
QT += core gui widgets
QT += core-private gui-private widgets-private

TEMPLATE = app
TARGET = tst_backuprestore

INCLUDEPATH += /opt/Qt/6.8.1/gcc_64/include/QtCore/6.8.1/QtCore/private
DEPENDPATH += /opt/Qt/6.8.1/gcc_64/include/QtCore/6.8.1/QtCore/private
INCLUDEPATH += ../

SOURCES += test.cpp \
           ../BackupManager.cpp \
           ../RestoreManager.cpp \
           ../ZipUtils.cpp

HEADERS += ../BackupManager.h \
           ../RestoreManager.h \
           ../ZipUtils.h 