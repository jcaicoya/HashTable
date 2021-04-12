TEMPLATE = lib
CONFIG += staticlib

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ActionList.cpp \
    ActionListModel.cpp \
    ActionListWidget.cpp \
    ActionManagerWidget.cpp \
    ActionResult.cpp \
    ActionResultWidget.cpp \
    ExecuteActionWidget.cpp \
    HashFunction.cpp \
    HashTableFactory.cpp \
    HashTableManagerWidget.cpp \
    HashTableModel.cpp \
    HashTableWidget.cpp \
    MainWindow.cpp \
    RehashFunction.cpp

HEADERS += \
    Action.h \
    ActionList.h \
    ActionListModel.h \
    ActionListWidget.h \
    ActionManagerWidget.h \
    ActionResult.h \
    ActionResultWidget.h \
    Constants.h \
    ExecuteActionWidget.h \
    HashFunction.h \
    HashTable.h \
    HashTableFactory.h \
    HashTableManagerWidget.h \
    HashTableModel.h \
    HashTableWidget.h \
    MainWindow.h \
    RehashFunction.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/lib
else: unix:!android: target.path = /opt/$${TARGET}/lib
!isEmpty(target.path): INSTALLS += target
