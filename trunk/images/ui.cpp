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
QImage svgNeedle;
QImage svgNeedle2;

void LoadImages()
{
    svgClock.load(QString(UIDIR "clock.svg"));
    svgAltitude.load(QString(UIDIR "speed10.svg"));
    svgHeading.load(QString(UIDIR "compass.svg"));
    svgSatView.load(QString(UIDIR "satview.svg"));
    svgSpeed10.load(QString(UIDIR "speed10.svg"));
    svgSpeed200.load(QString(UIDIR "speed200.svg"));
    
    svgLong.load(QString(UIDIR "longhand.svg"));
    svgShort.load(QString(UIDIR "shorthand.svg"));
    svgSecond.load(QString(UIDIR "secondhand.svg"));
    svgNeedle.load(QString(UIDIR "compassneedle.svg"));
    svgNeedle2.load(QString(UIDIR "speedneedle.svg"));
}
