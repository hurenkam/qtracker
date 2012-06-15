#-------------------------------------------------
#
# Project created by QtCreator 2011-08-17T11:14:06
#
#-------------------------------------------------

TEMPLATE  = app
TARGET    = tripdataserver

QT       += core
QT       += network
QT       += sql
QT       -= gui
QT       += declarative

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += mobility

MOBILITY  = \
    serviceframework \
    location

HEADERS  += \
    interface/tripservercommands.h \
    interface/command.h \
    interface/qmldatabase.h \
    interface/qmltrip.h \
    interface/qmlcategory.h \
    interface/qmlwaypoint.h \
    interface/qmlrefpoint.h \
    interface/qmlroute.h \
    interface/qmltrack.h \
    interface/qmlmap.h \
    tripdataserver.h \
    dataprovider.h \
    altitudeprovider.h \
    speedprovider.h \
    locationprovider.h \
    commandhandler.h \
    courseprovider.h \
    interface/helpers.h

SOURCES  += \
    interface/tripservercommands.cpp \
    interface/command.cpp \
    interface/qmldatabase.cpp \
    interface/qmltrip.cpp \
    interface/qmlcategory.cpp \
    interface/qmlwaypoint.cpp \
    interface/qmlrefpoint.cpp \
    interface/qmlroute.cpp \
    interface/qmltrack.cpp \
    interface/qmlmap.cpp \
    main.cpp \
    tripdataserver.cpp \
    locationprovider.cpp \
    altitudeprovider.cpp \
    speedprovider.cpp \
    commandhandler.cpp \
    courseprovider.cpp

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/src/eu/licentia/necessitas/industrius/QtActivity.java \
    android/src/eu/licentia/necessitas/industrius/QtApplication.java \
    android/src/eu/licentia/necessitas/industrius/QtLayout.java \
    android/src/eu/licentia/necessitas/industrius/QtSurface.java \
    android/src/eu/licentia/necessitas/ministro/IMinistro.aidl \
    android/src/eu/licentia/necessitas/ministro/IMinistroCallback.aidl \
    android/src/eu/licentia/necessitas/mobile/QtAndroidContacts.java \
    android/src/eu/licentia/necessitas/mobile/QtCamera.java \
    android/src/eu/licentia/necessitas/mobile/QtFeedback.java \
    android/src/eu/licentia/necessitas/mobile/QtLocation.java \
    android/src/eu/licentia/necessitas/mobile/QtMediaPlayer.java \
    android/src/eu/licentia/necessitas/mobile/QtSensors.java \
    android/src/eu/licentia/necessitas/mobile/QtSystemInfo.java
