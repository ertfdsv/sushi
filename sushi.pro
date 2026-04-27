# QT项目配置文件
QT       += core gui

# Qt版本大于4时添加widgets模块
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 项目目标名称
TARGET = sushi
# 项目模板类型（应用程序）
TEMPLATE = app

# 定义过时警告
DEFINES += QT_DEPRECATED_WARNINGS

# 源文件列表
SOURCES += \
        main.cpp \
        mainwindow.cpp \
        SushiItem.cpp

# 头文件列表
HEADERS += \
        mainwindow.h \
        SushiItem.h

# 表单文件列表（暂无）
FORMS += \

# 资源文件列表（暂无）
RESOURCES += \

# C++11标准支持
QMAKE_CXXFLAGS += -std=c++11

# Qt 6 兼容性配置
QT_CONFIG -= no-pkg-config
