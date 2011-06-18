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
    commandhandler.cpp \
    tripserver.cpp

OTHER_FILES += \
    qtrackerd.qcrml

VERSION = 0.4.409
DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"

symbian {
    # Remove all the existing platform dependencies
    default_deployment.pkg_prerules -= pkg_platform_dependencies

    #Add a dependency for just the S60 5th edition (Symbian^1) and later phones
    supported_platforms = \
        "; Application that only supports S60 5th edition" \
        "[0x1028315F],0,0,0,{\"S60ProductID\"}"

    platforms.pkg_prerules += supported_platforms
    DEPLOYMENT += platforms

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
    commandhandler.h \
    tripserver.h
