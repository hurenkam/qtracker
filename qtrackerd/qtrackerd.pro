#-------------------------------------------------
#
# Project created by QtCreator 2011-03-08T10:11:41
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += sql

TARGET = qtrackerd
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += mobility
MOBILITY += \
    publishsubscribe \
    sensors \
    location

TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    compassserver.cpp \
    locationserver.cpp \
    altitudeserver.cpp \
    speedserver.cpp \
    timeserver.cpp \
    monitorserver.cpp \
    satelliteserver.cpp \
    dataserver.cpp \
    track.cpp \
    commandhandler.cpp

OTHER_FILES += \
    qtrackerd.qcrml

VERSION = 0.3.331
DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"

symbian {
    TARGET.UID3 = 0xE5371b58
    TARGET.CAPABILITY += NetworkServices Location LocalServices UserEnvironment ReadUserData WriteUserData

    crml.sources = qtrackerd.qcrml
    crml.path = c:/resource/qt/crml
    DEPLOYMENT += crml
}

HEADERS += \
    server.h \
    compassserver.h \
    locationserver.h \
    altitudeserver.h \
    speedserver.h \
    timeserver.h \
    monitorserver.h \
    satelliteserver.h \
    helpers.h \
    dataserver.h \
    track.h \
    commandhandler.h
