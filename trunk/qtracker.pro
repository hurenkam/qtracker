CONFIG += mobility
MOBILITY += location \
    sensors \
    systeminfo
TEMPLATE = app
TARGET = qTracker
QT += core \
    gui \
    xml \
    network
HEADERS += \
    application/qtracker.h \
    dialogs/qdoubleedit.h \
    dialogs/qtrackdialog.h \
    dialogs/qroutedialog.h \
    dialogs/qwaypointdialog.h \
    dialogs/qmapdialog.h \
    dialogs/qcompassdialog.h \
    dialogs/qclockdialog.h \
    datums/datumlist.h \
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
    widgets/qmapwidget.h \
    widgets/qlevelwidget.h \
    widgets/qmapstatusbar.h
INCLUDEPATH += dialogs \
    datatypes \
    widgets \
    images \
    mobility \
    windows \
    application \
    proj4
FORMS += 
RESOURCES += 
VERSION = 0.8.264
VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"
symbian {
    TARGET.UID3 = 0xEA82CEF3
    LIBS += -llbs \
        -lefsrv \
        -leikcore \
        -lcone
    TARGET.CAPABILITY = Location
    DEFINES += LOCATION
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
        images/route.svg \
        images/import.svg \
        images/export.svg \
        images/exit.svg \
        images/delete.svg \
        images/edit.svg \
        images/visible.svg \
        images/invisible.svg \
        images/locator_red.svg \
        images/locator_green.svg \
        images/wpt_green.svg \
        images/compassring.svg \
        images/level.svg \
        images/leveltop.svg
    UiFiles.path = ./ui
    DEPLOYMENT += UiFiles
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
    dialogs/qclockdialog.cpp \
    dialogs/qtrackdialog.cpp \
    dialogs/qroutedialog.cpp \
    datums/datumlist.cpp \
    widgets/qgaugewidget.cpp \
    widgets/qmapwidget.cpp \
    widgets/qlevelwidget.cpp \
    widgets/qmapstatusbar.cpp \
    images/ui.cpp \
    widgets/qaltitudewidget.cpp \
    widgets/qclockwidget.cpp \
    widgets/qheadingwidget.cpp \
    widgets/qsatviewwidget.cpp \
    widgets/qspeedwidget.cpp \
    windows/qdashwindow.cpp \
    proj4/pj_param.c \
    proj4/adjlon.c \
    proj4/dmstor.c \
    proj4/geocent.c \
    proj4/nad_cvt.c \
    proj4/nad_init.c \
    proj4/nad_intr.c \
    proj4/pj_apply_gridshift.c \
    proj4/pj_datum_set.c \
    proj4/pj_datums.c \
    proj4/pj_ell_set.c \
    proj4/pj_ellps.c \
    proj4/pj_errno.c \
    proj4/pj_fwd.c \
    proj4/pj_gauss.c \
    proj4/pj_gridinfo.c \
    proj4/pj_gridlist.c \
    proj4/pj_init.c \
    proj4/pj_initcache.c \
    proj4/pj_inv.c \
    proj4/pj_latlong.c \
    proj4/pj_list.c \
    proj4/pj_malloc.c \
    proj4/pj_mlfn.c \
    proj4/pj_mutex.c \
    proj4/pj_open_lib.c \
    proj4/PJ_stere.c \
    proj4/PJ_sterea.c \
    proj4/PJ_tmerc.c \
    proj4/pj_transform.c \
    proj4/pj_tsfn.c \
    proj4/pj_units.c \
    main.cpp
