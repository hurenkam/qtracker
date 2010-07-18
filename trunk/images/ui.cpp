/*
 * ui.cpp
 *
 *  Created on: 4-okt-2009
 *      Author: Mark
 */

#include "ui.h"

QImage svgClock;
QImage svgAltitude;
QImage svgHeading;
QImage svgSatView;
QImage svgSpeed10;
QImage svgSpeed200;

QImage svgLong;
QImage svgShort;
QImage svgSecond;
QImage svgCompassNeedle;
QImage svgSpeedNeedle;

#ifdef Q_OS_SYMBIAN
#include "f32file.h" // efsrv.lib
#include "eikenv.h" // eikcore.lib
#include "eikappui.h" // eikcore.lib
#include "eikapp.h" // eikcore.lib

QString GetDrive()
{
        TParsePtrC appparser(CEikonEnv::Static()->EikAppUi()->Application()->AppFullName());
        TPtrC drive = appparser.Drive();
        //if (!inemulator)
        return QString::fromUtf16(drive.Ptr(),drive.Length());
        //return QString("C:");
}
#else
QString GetDrive()
{
    return QString("");
}
#endif

void LoadImages()
{
    svgClock.load(":/dash/clock.svg");
    svgAltitude.load(":/dash/speed10.svg");
    svgHeading.load(":/dash/compass.svg");
    svgSatView.load(":/dash/satview.svg");
    svgSpeed10.load(":/dash/speed10.svg");
    svgSpeed200.load(":/dash/speed200.svg");

    svgLong.load(":/dash/longhand.svg");
    svgShort.load(":/dash/shorthand.svg");
    svgSecond.load(":/dash/secondhand.svg");
    svgCompassNeedle.load(":/dash/compassneedle.svg");
    svgSpeedNeedle.load(":/dash/speedneedle.svg");
}
