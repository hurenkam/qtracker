#include "trackstatus.h"
#include "qmltrack.h"

#define ENABLE_DEBUG
#include "helpers.h"

TrackStatus::TrackStatus(QObject *parent)
    : QObject(parent)
    , id(0)
    , name("track")
    , state("idle")
    , _track(0)
{
    ENTER("")
}

void TrackStatus::onPositionChanged(const QGeoPositionInfo& info)
{
    if ( (state=="recording") &&
          info.isValid() &&
          info.coordinate().isValid() )
    {
        QGeoCoordinate c = info.coordinate();
        QDateTime time = QDateTime::currentDateTime();
        double lat = c.latitude();
        double lon = c.longitude();
        double alt = c.altitude();

        if (!_pc.isValid()) _pc = c;
        if (!_pt.isValid()) _pt = time;

        int delta = _track->interval();
        double dt = _pt.secsTo(QDateTime::currentDateTime());
        double dc = _pc.distanceTo(c);

        ENTER(lat << lon << alt << delta << dt << dc )
        if ((delta == 0) ||
           ((delta < 0) && (dc >= -1 * delta)) ||
           ((delta > 0) && (dt >=      delta)) )
        {
            _pc = c;
            _pt = time;
            emit trackPoint(lat,lon,alt);
        }
    }
}

void TrackStatus::registerApi(XmlRpcServer* srv)
{
    ENTER("")
    srv->registerSlot( this,    SLOT( start(const QVariant&, const QVariant&) ), "/RPC2/track/" );
    srv->registerSlot( this,    SLOT( stop() ),                                  "/RPC2/track/" );
    srv->registerSlot( this,    SLOT( data() ),                                  "/RPC2/track/" );
}

void TrackStatus::save()
{
    if (_track)
        _track->save();
}

QVariant TrackStatus::start(const QVariant& id, const QVariant& name)
{
    ENTER(id << name)
    if (_track) return QVariant(-1);

    _track = new qmlTrack(id.toInt());
    _track->setName(name.toString());
    _track->save();
    this->id = _track->trkid();
    this->name = _track->name();
    this->state = "recording";

    connect(this,SIGNAL(trackPoint(double,double,double)), _track, SLOT(addTrackPoint(double,double,double)));

    return QVariant(0);
}

QVariant TrackStatus::stop()
{
    ENTER("")
    delete (_track);
    _track = 0;
    id = 0;
    name = "track";
    state = "idle";
    return QVariant(0);
}

QVariant TrackStatus::data()
{
    ENTER("")
    QMap<QString, QVariant> r;
    r["id"]    = id;
    r["name"]  = name;
    r["state"] = state;
    return r;
}
