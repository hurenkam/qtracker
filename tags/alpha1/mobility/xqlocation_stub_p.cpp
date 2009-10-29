// INCLUDE FILES
#include <QMainWindow>
#include <QTimer>
#include "xqlocation.h"
#include "xqlocation_stub_p.h"
#include "locationsimulator.h"

#include <iostream>
using namespace std;
//#define LOG( a ) cout << a
#define LOG( a )


XQLocationPrivate::XQLocationPrivate(XQLocation * aParent)
: updateInterval(1000) // default update interval is one second
, iParent(aParent)
{
    //w.show();
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(emitPositionUpdates()));
    t->start(1000);
}

void XQLocationPrivate::emitPositionUpdates()
{
    double lat = 51.4666 + 0.0038;
    double lon = 5.4833 + 0.00356;
    double alt = 63;
    float speed = 0;
    float head = 0;
    emit iParent->locationChanged(lat,lon,alt,speed,head);
    LOG( "emit position: " << lat <<" "<< lon << "\n"; )
}

XQLocationPrivate::~XQLocationPrivate()
{
    //w.close();
}

void XQLocationPrivate::requestUpdate()
{
}

void XQLocationPrivate::startUpdates(int msec)
{
    updateInterval = msec;
}

void XQLocationPrivate::startUpdates()
{
}

void XQLocationPrivate::stopUpdates()
{
}

XQLocation::Error XQLocationPrivate::openConnectionToPositioner()
{
    return XQLocation::NoError;
}

void XQLocationPrivate::closeConnectionToPositioner()
{
}

// End of file
