#include <QObject>
#include <QDebug>
#include "positionmodel.h"


//#define ENABLE_DEBUG
#include "helpers.h"

//============================================================================================


PrivatePositionModel::PrivatePositionModel(QObject *parent)
    : QObject(parent)
    , _latitude(0)
    , _longitude(0)
    , _altitude(0)
{
    ENTER("")

    possource = QGeoPositionInfoSource::createDefaultSource(this);
    if (possource) {
        possource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
        possource->setUpdateInterval(500);
        connect(possource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(OnPositionUpdate(QGeoPositionInfo)));
        possource->startUpdates();
    }

    EXIT("")
}

void PrivatePositionModel::OnPositionUpdate(const QGeoPositionInfo& info)
{
    ENTER(info)

    posinfo = info;
    _latitude = posinfo.coordinate().latitude();
    _longitude = posinfo.coordinate().longitude();
    emit latitudeChanged();
    emit longitudeChanged();
    emit positionChanged();
    double alt = posinfo.coordinate().altitude();
    if (alt == alt) // check for NaN
    {
        _altitude = alt;
        emit altitudeChanged();
    }
    UpdateQuality();

    EXIT("")
}

void PrivatePositionModel::UpdateQuality()
{
    ENTER("")

    if (posinfo.hasAttribute(QGeoPositionInfo::VerticalAccuracy))
    {
        int r = posinfo.attribute(QGeoPositionInfo::VerticalAccuracy);
        if (r < 300)
        {
            _vertical = 100 - r/3;
            emit verticalChanged();
        }
    }
    if (posinfo.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
    {
        int r = posinfo.attribute(QGeoPositionInfo::HorizontalAccuracy);
        if (r < 300)
        {
            _horizontal = 100 - r/3;
            emit horizontalChanged();
        }
    }

    EXIT("")
}


//============================================================================================


PrivatePositionModel *PositionModel::p = 0;

PositionModel::PositionModel(QObject *parent): QObject(parent)
{
    ENTER("")

    if (!p) p = new PrivatePositionModel();

    connect(p, SIGNAL(latitudeChanged()),   this, SLOT(onLatitudeChanged()));
    connect(p, SIGNAL(longitudeChanged()),  this, SLOT(onLongitudeChanged()));
    connect(p, SIGNAL(altitudeChanged()),   this, SLOT(onAltitudeChanged()));
    connect(p, SIGNAL(positionChanged()),   this, SLOT(onPositionChanged()));
    connect(p, SIGNAL(verticalChanged()),   this, SLOT(onVerticalChanged()));
    connect(p, SIGNAL(horizontalChanged()), this, SLOT(onHorizontalChanged()));

    EXIT("")
}

double PositionModel::latitude()
{
    ENTER("")
    RETURN(double,p->latitude())
}

double PositionModel::longitude()
{
    ENTER("")
    RETURN(double,p->longitude())
}

double PositionModel::altitude()
{
    ENTER("")
    RETURN(double,p->altitude())
}

double PositionModel::horizontal()
{
    ENTER("")
    RETURN(double,p->horizontal())
}

double PositionModel::vertical()
{
    ENTER("")
    RETURN(double,p->vertical())
}

QString PositionModel::position()
{
    ENTER("")
    RETURN(QString,p->position())
}
