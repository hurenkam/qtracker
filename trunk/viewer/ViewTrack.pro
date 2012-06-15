# Add more folders to ship with the application, here
main.source = main
main.target = .
components.source = components
components.target = .
model.source = model
model.target = .
gauges.source = gauges
gauges.target = .
DEPLOYMENTFOLDERS = main components model gauges

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE4440A27

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
CONFIG += mobility
MOBILITY += sensors location

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
# CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

QT += sql

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    main.cpp \
    tripdatabase/qmlcategory.cpp \
    tripdatabase/qmldatabase.cpp \
    tripdatabase/qmlmap.cpp \
    tripdatabase/qmlrefpoint.cpp \
    tripdatabase/qmlroute.cpp \
    tripdatabase/qmltrack.cpp \
    tripdatabase/qmltrip.cpp \
    tripdatabase/qmlwaypoint.cpp

INCLUDEPATH += tripdatabase
HEADERS += \
    tripdatabase/qmlcategory.h \
    tripdatabase/qmldatabase.h \
    tripdatabase/qmlmap.h \
    tripdatabase/qmlrefpoint.h \
    tripdatabase/qmlroute.h \
    tripdatabase/qmltrack.h \
    tripdatabase/qmltrip.h \
    tripdatabase/qmlwaypoint.h

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    android/version.xml \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/res/layout/splash.xml \
    android/res/values-ru/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-nb/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values-nl/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/AndroidManifest.xml \
    android/version.xml \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/res/layout/splash.xml \
    android/res/values-ru/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values/strings.xml \
    android/res/values/libs.xml \
    android/res/values-nb/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values-nl/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable/icon.png \
    android/AndroidManifest.xml

RESOURCES += \
    components.qrc \
    model.qrc \
    main.qrc \
    gauges.qrc


