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

//QImage svgMap;
//QImage jpgMap;

#ifdef Q_OS_SYMBIAN
#include "f32file.h" // efsrv.lib
#include "eikenv.h" // eikcore.lib
#include "eikappui.h" // eikcore.lib
#include "eikapp.h" // eikcore.lib

QString GetDrive()
{
        TParsePtrC appparser(CEikonEnv::Static()->EikAppUi()->Application()->AppFullName());
        TPtrC drive = appparser.Drive();
        //if (drive.Length() > 0)
        return QString::fromUtf16(drive.Ptr(),drive.Length());
        //return QString("C:");
}
QString GetDriveAndPath()
{
        TParsePtrC appparser(CEikonEnv::Static()->EikAppUi()->Application()->AppFullName());
        TPtrC driveandpath = appparser.DriveAndPath();
		return QString::fromUtf16(driveandpath.Ptr(),driveandpath.Length());
}
#else
QString GetDrive()
{
    return QString("");
}
QString GetDriveAndPath()
{
    return QString("/Users/hurenkam/workspace/qtracker/");
}
#endif

void LoadImages()
{
    svgClock.load(GetDrive() + QString(UIDIR "clock.svg"));
    svgAltitude.load(GetDrive() + QString(UIDIR "speed10.svg"));
    svgHeading.load(GetDrive() + QString(UIDIR "compass.svg"));
    svgSatView.load(GetDrive() + QString(UIDIR "satview.svg"));
    svgSpeed10.load(GetDrive() + QString(UIDIR "speed10.svg"));
    svgSpeed200.load(GetDrive() + QString(UIDIR "speed200.svg"));

    svgLong.load(GetDrive() + QString(UIDIR "longhand.svg"));
    svgShort.load(GetDrive() + QString(UIDIR "shorthand.svg"));
    svgSecond.load(GetDrive() + QString(UIDIR "secondhand.svg"));
    svgCompassNeedle.load(GetDrive() + QString(UIDIR "compassneedle.svg"));
    svgSpeedNeedle.load(GetDrive() + QString(UIDIR "speedneedle.svg"));
}

/* See also:
 * http://www.mobilenme.com/content/view/48/26/
 * http://discussion.forum.nokia.com/forum/showthread.php?t=114197

QString GetInstallationDrive()
{
    TParsePtrC appparser(Application()->AppFullName());
    TParse parser;
    TPtrC drive = appparser.Drive();

    QString text = QString::fromUtf16(textFromInterface.Ptr(),
        textFromInterface.Length());
}
*/
