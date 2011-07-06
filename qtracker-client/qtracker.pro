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

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH = /imports

# Avoid auto screen rotation
#DEFINES += ORIENTATIONLOCK

# Needs to be defined for Symbian
#DEFINES += NETWORKACCESS
QT       += sql

VERSION = 0.4.422

symbian {
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
win32 {
    DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"
}
unix {
    DEFINES += VERSION=\"\\\"$${VERSION}\\\"\"
}


# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += location \
    sensors \
    publishsubscribe \
    systeminfo

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    cpp/folderlistmodel.cpp \
    cpp/deviceinfomodel.cpp \
    cpp/main.cpp \
    cpp/mapview.cpp \
    cpp/client.cpp \
    cpp/gpxfile.cpp \
    cpp/database.cpp \
    cpp/qmlwaypoint.cpp \
    cpp/qmlroute.cpp \
    cpp/qmltrack.cpp \
    cpp/qmltrip.cpp \
    cpp/qmlcategory.cpp \
    cpp/qmlmap.cpp \
    cpp/qmlrefpoint.cpp \
    cpp/qmldatabase.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    cpp/folderlistmodel.h \
    cpp/deviceinfomodel.h \
    cpp/mapview.h \
    cpp/helpers.h \
    cpp/client.h \
    cpp/gpxfile.h \
    cpp/database.h \
    cpp/qmlwaypoint.h \
    cpp/qmlroute.h \
    cpp/qmltrack.h \
    cpp/qmltrip.h \
    cpp/qmlcategory.h \
    cpp/qmlmap.h \
    cpp/qmlrefpoint.h \
    cpp/qmldatabase.h

OTHER_FILES += \
