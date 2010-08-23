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
    dialogs/qclockdialog.h \
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
    geographiclib/AzimuthalEquidistant.hpp \
    geographiclib/cassinisoldner.hpp \
    geographiclib/constants.hpp \
    geographiclib/dms.hpp \
    geographiclib/ellipticfunction.hpp \
    geographiclib/geocentric.hpp \
    geographiclib/geocoords.hpp \
    geographiclib/geodesic.hpp \
    geographiclib/geoid.hpp \
    geographiclib/gnomonic.hpp \
    geographiclib/lambertconformalconic.hpp \
    geographiclib/localcartesian.hpp \
    geographiclib/mgrs.hpp \
    geographiclib/polarstereographic.hpp \
    geographiclib/transversemercator.hpp \
    geographiclib/transversemercatorexact.hpp \
    geographiclib/utmups.hpp 
INCLUDEPATH += dialogs \
    datatypes \
    widgets \
    images \
    mobility \
    windows \
    application \
    geographiclib
FORMS += 
RESOURCES += 
VERSION = 0.8.234
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
    VERSION = 0.8.250
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
    dialogs/qclockdialog.cpp \
    dialogs/qtrackdialog.cpp \
    dialogs/qroutedialog.cpp \
    widgets/qgaugewidget.cpp \
    widgets/qmapwidget.cpp \
    widgets/qlevelwidget.cpp \
    images/ui.cpp \
    widgets/qaltitudewidget.cpp \
    widgets/qclockwidget.cpp \
    widgets/qheadingwidget.cpp \
    widgets/qsatviewwidget.cpp \
    widgets/qspeedwidget.cpp \
    windows/qdashwindow.cpp \
    geographiclib/AzimuthalEquidistant.cpp \
    geographiclib/CassiniSoldner.cpp \
    geographiclib/DMS.cpp \
    geographiclib/EllipticFunction.cpp \
    geographiclib/Geocentric.cpp \
    geographiclib/GeoCoords.cpp \
    geographiclib/Geodesic.cpp \
    geographiclib/Geoid.cpp \
    geographiclib/Gnomonic.cpp \
    geographiclib/LambertConformalConic.cpp \
    geographiclib/LocalCartesian.cpp \
    geographiclib/MGRS.cpp \
    geographiclib/PolarStereographic.cpp \
    geographiclib/TransverseMercator.cpp \
    geographiclib/TransverseMercatorExact.cpp \
    geographiclib/UTMUPS.cpp \
    main.cpp
