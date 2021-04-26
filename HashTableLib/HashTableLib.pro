TEMPLATE = lib
CONFIG += staticlib

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ExampleTabWidget.cpp \
    ExampleWidget.cpp \
    OperationList.cpp \
    OperationListModel.cpp \
    OperationListWidget.cpp \
    OperationManagerWidget.cpp \
    OperationResult.cpp \
    OperationResultWidget.cpp \
    AddHashTableDialog.cpp \
    ExecuteOperationWidget.cpp \
    HashFunction.cpp \
    HashTableDescriptionWidget.cpp \
    HashTableManagerWidget.cpp \
    HashTableModel.cpp \
    HashTableWidget.cpp \
    MainWindow.cpp \
    RehashFunction.cpp \
    RemoveHashTableDialog.cpp

HEADERS += \
    ExampleTabWidget.h \
    ExampleWidget.h \
    HashTable.h \
    MatrixHashTable.h \
    Operation.h \
    OperationList.h \
    OperationListModel.h \
    OperationListWidget.h \
    OperationManagerWidget.h \
    OperationResult.h \
    OperationResultWidget.h \
    AddHashTableDialog.h \
    ArrayHashTable.h \
    Constants.h \
    ExecuteOperationWidget.h \
    HashFunction.h \
    HashTableDefinition.h \
    HashTableDescriptionWidget.h \
    HashTableFactory.h \
    HashTableManagerWidget.h \
    HashTableModel.h \
    HashTableWidget.h \
    MainWindow.h \
    RehashFunction.h \
    RemoveHashTableDialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/lib
else: unix:!android: target.path = /opt/$${TARGET}/lib
!isEmpty(target.path): INSTALLS += target
