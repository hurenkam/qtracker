TEMPLATE = app
TARGET = qTracker
QT += core \
    gui
HEADERS += location_global.h \
    xqlocation.h \
    xqlocation_s60_p.h \
    qdashwindow.h \
    ui.h \
    qaltitudewidget.h \
    qclockwidget.h \
    qheadingwidget.h \
    qsatviewwidget.h \
    qspeedwidget.h
SOURCES += xqlocation.cpp \
    xqlocation_s60_p.cpp \
    qdashwindow.cpp \
    main.cpp \
    ui.cpp \
    qaltitudewidget.cpp \
    qclockwidget.cpp \
    qheadingwidget.cpp \
    qsatviewwidget.cpp \
    qspeedwidget.cpp
FORMS += 
RESOURCES += 
symbian { 
    TARGET.UID3 = 0xEA82CEF3
    LIBS += -llbs
    TARGET.CAPABILITY = Location \
        LocalServices \
        NetworkServices \
        PowerMgmt \
        ProtServ \
        ReadDeviceData \
        ReadUserData \
        SurroundingsDD \
        SwEvent \
        TrustedUI \
        UserEnvironment \
        WriteDeviceData \
        WriteUserData
    ICON = icon.svg
    UiFiles.sources += Ui\LongHand.svg \
        Ui\ShortHand.svg \
        Ui\SecondHand.svg \
        Ui\Clock.svg \
        Ui\Speed10.svg \
        Ui\Speed200.svg \
        Ui\SpeedNeedle.svg \
        Ui\CompassNeedle.svg \
        Ui\Compass.svg \
        Ui\SatView.svg \
        Ui\style.css
    UiFiles.path = .
    DEPLOYMENT += UiFiles
    VERSION = 1.0.1
}
