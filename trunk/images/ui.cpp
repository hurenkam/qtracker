/*
 * ui.cpp
 *
 *  Created on: 4-okt-2009
 *      Author: Mark
 */

#include "ui.h"

QImage svgClock;
QImage svgCompassRing;
QImage svgLevel;
QImage svgLevelTop;
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
    svgClock.load(DASHRCDIR "clock.svg");
    svgCompassRing.load(DASHRCDIR "compassring.svg");
    svgLevel.load(DASHRCDIR "level.svg");
    svgLevelTop.load(DASHRCDIR "leveltop.svg");
    svgAltitude.load(DASHRCDIR "speed10.svg");
    svgHeading.load(DASHRCDIR "compass.svg");
    svgSatView.load(DASHRCDIR "satview.svg");
    svgSpeed10.load(DASHRCDIR "speed10.svg");
    svgSpeed200.load(DASHRCDIR "speed200.svg");

    svgLong.load(DASHRCDIR "longhand.svg");
    svgShort.load(DASHRCDIR "shorthand.svg");
    svgSecond.load(DASHRCDIR "secondhand.svg");
    svgCompassNeedle.load(DASHRCDIR "compassneedle.svg");
    svgSpeedNeedle.load(DASHRCDIR "speedneedle.svg");
}
