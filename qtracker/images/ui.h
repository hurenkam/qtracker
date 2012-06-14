/*
 * ui.h
 *
 *  Created on: 4-okt-2009
 *      Author: Mark
 */

#ifndef UI_H_
#define UI_H_

#include <QtGui>
extern QImage svgClock;
extern QImage svgAltitude;
extern QImage svgHeading;
extern QImage svgSatView;
extern QImage svgSpeed10;
extern QImage svgSpeed200;

extern QImage svgLong;
extern QImage svgShort;
extern QImage svgSecond;
extern QImage svgCompassNeedle;
extern QImage svgCompassRing;
extern QImage svgLevel;
extern QImage svgLevelTop;
extern QImage svgSpeedNeedle;

#ifdef USE_QT_RESOURCES
#define DASHRCDIR   ":/dash/"
#define MAPRCDIR    ":/map/"
#define CSSRCDIR    ":/css/"
#define SPLASHRCDIR ":/splash/"
#define DIALOGRCDIR ":/dialog/"
#else
#ifdef Q_OS_SYMBIAN
#define UIDIR        "/private/EA82CEF3/ui/"
#define TEXTDIVIDER 38
#define DASHRCDIR    GetDrive() + UIDIR
#define MAPRCDIR     GetDrive() + UIDIR
#define CSSRCDIR     GetDrive() + UIDIR
#define SPLASHRCDIR  GetDrive() + UIDIR
#define DIALOGRCDIR  GetDrive() + UIDIR
#else
#define TEXTDIVIDER 38
#define DASHRCDIR    UIDIR
#define MAPRCDIR     UIDIR
#define CSSRCDIR     UIDIR
#define SPLASHRCDIR  UIDIR
#define DIALOGRCDIR  UIDIR
#endif
#endif

void LoadImages();
QString GetDrive();
QString GetDriveAndPath();
#endif /* UI_H_ */
