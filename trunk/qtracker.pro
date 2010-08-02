CONFIG += mobility
MOBILITY += location \
    sensors \
    systeminfo
TEMPLATE = app
TARGET = qTracker
QT += core \
    gui \
    xml \
    svg
HEADERS += \
    application/qtracker.h \
    dialogs/qdoubleedit.h \
    dialogs/qtrackdialog.h \
    dialogs/qroutedialog.h \
    dialogs/qwaypointdialog.h \
    dialogs/qmapdialog.h \
    dialogs/qcompassdialog.h \
    datatypes/geodata.h \
    datatypes/gpxio.h \
    datatypes/waypointlist.h \
    datatypes/routelist.h \
    datatypes/tracklist.h \
    datatypes/maplist.h \
    datatypes/datamonitor.h \
    images/ui.h \
    widgets/qaltitudewidget.h \
    widgets/qclockwidget.h \
    widgets/qheadingwidget.h \
    widgets/qsatviewwidget.h \
    widgets/qspeedwidget.h \
    windows/qdashwindow.h \
    widgets/qgaugewidget.h \
    widgets/qmapwidget.h
INCLUDEPATH += dialogs \
    datatypes \
    widgets \
    images \
    mobility \
    windows \
    application
FORMS += 
RESOURCES += 
symbian { 
    TARGET.UID3 = 0xEA82CEF3
    LIBS += -llbs \
        -lefsrv \
        -leikcore \
        -lcone
    TARGET.CAPABILITY = Location ReadDeviceData
    ICON = images/icon.svg
    UiFiles.sources += images/clock.svg \
        images/compass.svg \
        images/compassneedle.svg \
        images/longhand.svg \
        images/satview.svg \
        images/secondhand.svg \
        images/shorthand.svg \
        images/speed10.svg \
        images/speed200.svg \
        images/style.css \
        images/speedneedle.svg \
        images/map.svg \
        images/splash.png \
        images/zoom-in.svg \
        images/zoom-out.svg \
        images/home.svg \
        images/options.svg \
        images/statusbar.svg \
        images/flag.svg \
        images/hiker.svg \
        images/delete.svg \
        images/edit.svg \
        images/visible.svg \
        images/invisible.svg \
        images/locator_red.svg \
        images/locator_green.svg \
        images/wpt_green.svg \
        images/compassring.svg
    UiFiles.path = ./ui
    DEPLOYMENT += UiFiles
    VERSION = 0.8.211
	VERSTR = '\\"$${VERSION}\\"'    
	DEFINES += VER=\"$${VERSTR}\"
    TARGET.EPOCHEAPSIZE = 0x30000 \
        0x3000000
    TARGET.EPOCSTACKSIZE += 0x14000
}
SOURCES += \
    application/qtracker.cpp \
    datatypes/geodata.cpp \
    datatypes/gpxio.cpp \
    datatypes/waypointlist.cpp \
    datatypes/routelist.cpp \
    datatypes/tracklist.cpp \
    datatypes/maplist.cpp \
    datatypes/datamonitor.cpp \
    dialogs/qwaypointdialog.cpp \
    dialogs/qmapdialog.cpp \
    dialogs/qcompassdialog.cpp \
    dialogs/qtrackdialog.cpp \
    dialogs/qroutedialog.cpp \
    widgets/qgaugewidget.cpp \
    widgets/qmapwidget.cpp \
    images/ui.cpp \
    widgets/qaltitudewidget.cpp \
    widgets/qclockwidget.cpp \
    widgets/qheadingwidget.cpp \
    widgets/qsatviewwidget.cpp \
    widgets/qspeedwidget.cpp \
    windows/qdashwindow.cpp \
    main.cpp
