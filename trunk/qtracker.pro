CONFIG += mobility
MOBILITY += location \
    sensors
TEMPLATE = app
TARGET = qTracker
QT += core \
    gui \
    xml
HEADERS += dialogs/qwaypointdialog.h \
    datatypes/qmapmetadata.h \
    dialogs/qmapselectiondialog.h \
    widgets/qgaugewidget.h \
    widgets/qmapwidget.h \
    images/ui.h \
    widgets/qaltitudewidget.h \
    widgets/qclockwidget.h \
    widgets/qheadingwidget.h \
    widgets/qsatviewwidget.h \
    widgets/qspeedwidget.h \
    windows/qdashwindow.h
INCLUDEPATH += dialogs \
    datatypes \
    widgets \
    images \
    mobility \
    windows
FORMS += 
RESOURCES += 
symbian { 
    TARGET.UID3 = 0xEA82CEF3
    LIBS += -llbs \
        -lefsrv \
        -leikcore \
        -lcone
    TARGET.CAPABILITY = Location
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
        images/flag.svg
    UiFiles.path = ./ui
    DEPLOYMENT += UiFiles
    VERSION = 0.8.110
    TARGET.EPOCHEAPSIZE = 0x30000 \
        0x3000000
    TARGET.EPOCSTACKSIZE += 0x14000
}
SOURCES += dialogs/qwaypointdialog.cpp \
    datatypes/qmapmetadata.cpp \
    dialogs/qmapselectiondialog.cpp \
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
