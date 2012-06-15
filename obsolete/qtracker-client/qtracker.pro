# Add more folders to ship with the application, here
main.source = Main
main.target = .
components.source = Components
components.target = .
gauges.source = Gauges
gauges.target = .
cat.source = Category
cat.target = .
map.source = Map
map.target = .
wpt.source = Waypoint
wpt.target = .
rte.source = Route
rte.target = .
trk.source = Track
trk.target = .
trip.source = Trip
trip.target = .
img.source = Images
img.target = .
DEPLOYMENTFOLDERS = cat main components gauges map wpt rte trk trip img

# custom target 'doc' in *.pro file
MOCSOURCES = altitudemodel.cpp
moc.commands = moc $$MOCSOURCES
moc.depends = $$MOCSOURCES
QMAKE_EXTRA_UNIX_TARGETS += moc

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Avoid auto screen rotation
#DEFINES += ORIENTATIONLOCK

INCLUDEPATH += cpp/interface
INCLUDEPATH += cpp/database

# Needs to be defined for Symbian
DEFINES += NETWORKACCESS
QT       += sql
QT       += svg

VERSION = 0.4.423

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += location \
    sensors \
    systeminfo

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    cpp/folderlistmodel.cpp \
    cpp/deviceinfomodel.cpp \
    cpp/main.cpp \
    cpp/mapview.cpp \
    cpp/gpxfile.cpp \
    cpp/database/qmlwaypoint.cpp \
    cpp/database/qmlroute.cpp \
    cpp/database/qmltrack.cpp \
    cpp/database/qmltrip.cpp \
    cpp/database/qmlcategory.cpp \
    cpp/database/qmlmap.cpp \
    cpp/database/qmlrefpoint.cpp \
    cpp/database/qmldatabase.cpp

TCP_SOURCES = \
    cpp/interface/tripservercommands.cpp \
    cpp/interface/commandcaller.cpp \
    cpp/interface/command.cpp \
    cpp/tripserverinterface_tcp.cpp \
    cpp/altitudemodel_tcp.cpp \
    cpp/speedmodel_tcp.cpp \
    cpp/distancemodel_tcp.cpp \
    cpp/timemodel_tcp.cpp

PS_SOURCES = \
    cpp/altitudemodel_ps.cpp \
    cpp/speedmodel_ps.cpp \
    cpp/distancemodel_ps.cpp \
    cpp/timemodel_ps.cpp

HEADERS += \
    cpp/folderlistmodel.h \
    cpp/deviceinfomodel.h \
    cpp/mapview.h \
    cpp/helpers.h \
    cpp/gpxfile.h \
    cpp/qmlaltitudemodel.h \
    cpp/qmlspeedmodel.h \
    cpp/qmldistancemodel.h \
    cpp/qmltimemodel.h \
    cpp/interface/qmltripserverinterface.h \
    cpp/database/qmlwaypoint.h \
    cpp/database/qmlroute.h \
    cpp/database/qmltrack.h \
    cpp/database/qmltrip.h \
    cpp/database/qmlcategory.h \
    cpp/database/qmlmap.h \
    cpp/database/qmlrefpoint.h \
    cpp/database/qmldatabase.h

TCP_HEADERS = \
    cpp/interface/tripservercommands.h \
    cpp/interface/commandcaller.h \
    cpp/interface/command.h \
    cpp/tripserverinterface_tcp.h \
    cpp/altitudemodel_tcp.h \
    cpp/speedmodel_tcp.h \
    cpp/distancemodel_tcp.h \
    cpp/timemodel_tcp.h

PS_HEADERS = \
    cpp/altitudemodel_ps.h \
    cpp/speedmodel_ps.h \
    cpp/distancemodel_ps.h \
    cpp/timemodel_ps.h

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

win32 {
    DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"
}

symbian: {
    MOBILITY += publishsubscribe
    SOURCES  += $$PS_SOURCES
    HEADERS  += $$PS_HEADERS

    # Remove all the existing platform dependencies
    default_deployment.pkg_prerules -= pkg_platform_dependencies

    #Add a dependency for just the S60 5th edition (Symbian^1) and later phones
    supported_platforms = \
        "; Application that only supports S60 5th edition" \
        "[0x1028315F],0,0,0,{\"S60ProductID\"}"

    platforms.pkg_prerules += supported_platforms
    DEPLOYMENT += platforms

    DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"
    TARGET.UID3 = 0xE024B05A
    TARGET.CAPABILITY += NetworkServices Location LocalServices UserEnvironment ReadUserData WriteUserData
    TARGET.EPOCHEAPSIZE = 0x30000 0x3000000
}

android: {
    DEFINES += Q_OS_ANDROID
    DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"
    SOURCES += $$TCP_SOURCES
    HEADERS += $$TCP_HEADERS

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

    RESOURCES += \
        qml.qrc
}
