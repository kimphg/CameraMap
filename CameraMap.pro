#-------------------------------------------------
#
# Project created by QtCreator 2016-12-02T09:20:08
#
#-------------------------------------------------

QT += core
QT += core gui
QT += network

CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapSample
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cmap.cpp \
    c_config.cpp \
    c_camera.cpp \
    videoHandler/FeatureAnalyzer.cpp \
    videoHandler/FlameDecider.cpp \
    videoHandler/FlameDetector.cpp \
    videoHandler/TargetExtractor.cpp \
    videoHandler/utils.cpp \
    videoHandler/VideoHandler.cpp \
    videoHandler/videowork.cpp

HEADERS  += mainwindow.h \
    cmap.h \
    c_config.h \
    c_camera.h \
    videoHandler/common.h \
    videoHandler/FeatureAnalyzer.h \
    videoHandler/FlameDecider.h \
    videoHandler/FlameDetector.h \
    videoHandler/TargetExtractor.h \
    videoHandler/utils.h \
    videoHandler/VideoHandler.h \
    videoHandler/videowork.h

FORMS    += mainwindow.ui
win32:RC_ICONS += icon.ico
