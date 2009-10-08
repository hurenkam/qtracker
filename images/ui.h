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
extern QImage svgNeedle;
extern QImage svgNeedle2;

//#define UIDIR "c:/data/tracker040/ui/"
#define UIDIR "c:/private/EA82CEF3/ui/"

void LoadImages();
#endif /* UI_H_ */
