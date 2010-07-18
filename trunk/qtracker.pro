CONFIG += mobility
MOBILITY += location \
    sensors
TEMPLATE = app
TARGET = qTracker
QT += core \
    gui \
    xml
HEADERS += dialogs/qtrackdialog.h \
    datatypes/geodata.h \
    datatypes/gpxio.h \
    dialogs/qwaypointdialog.h \
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
RESOURCES += \ 
    images/ui.qrc
symbian { 
    TARGET.UID3 = 0xEA82CEF3
    LIBS += -llbs \
        -lefsrv \
        -leikcore \
        -lcone
    TARGET.CAPABILITY = Location
    ICON = images/icon.svg
    VERSION = 0.8.136
    TARGET.EPOCHEAPSIZE = 0x30000 \
        0x3000000
    TARGET.EPOCSTACKSIZE += 0x14000
}
SOURCES += dialogs/qtrackdialog.cpp \
    datatypes/geodata.cpp \
    datatypes/gpxio.cpp \
    dialogs/qwaypointdialog.cpp \
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
